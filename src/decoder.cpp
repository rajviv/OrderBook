// This is the implementation file for the Decoder class.

#include "../include/decoder.hpp"

// Function to read the next packet from the input file stream and create a new Packet structure
bool Decoder::readNextPacket(ifstream &ifs, Packet *&packet)
{
    packet = new Packet(); // Allocate memory for a new Packet structure
    PacketHeader &ph = packet->ph;
    if (!ifs.read(reinterpret_cast<char *>(&ph), 12)) // Reading the first 12 bytes (timestamp and numMessages) into the packet header
    {
        free(packet); // Free the memory if reading fails
        packet = nullptr;
        return false; // Return false to indicate the end of the file
    }

    vector<Message> &messages = packet->messages;
    messages.resize(ph.numMessages); // Resize the messages vector to accommodate the specified number of messages

    for (int i = 0; i < ph.numMessages; i++) // Loop through each message in the packet
    {
        ifs.read(reinterpret_cast<char *>(&(messages[i].msgType)), sizeof(messages[i].msgType)); // Read the message type (1 byte)

        switch (messages[i].msgType) // Depending on the message type, read the corresponding data
        {
        case 'N':                                                                                                        // New Message
            messages[i].newData = new NewMessage();                                                                      // Allocate memory for the new message data
            ifs.read(reinterpret_cast<char *>(&(messages[i].newData->price)), sizeof(messages[i].newData->price));       // Read price (8 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].newData->quantity)), sizeof(messages[i].newData->quantity)); // Read quantity (8 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].newData->symbolId)), sizeof(messages[i].newData->symbolId)); // Read symbolId (4 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].newData->orderId)), sizeof(messages[i].newData->orderId));   // Read orderId (8 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].newData->s)), sizeof(messages[i].newData->s));               // Read s (1 byte)
            messages[i].newData->side = int(messages[i].newData->s);                                                     // Convert s to int and store it as side
            break;
        case 'C':                                                                                                        // Cancel Message
        case 'T':                                                                                                        // Trade Message
            messages[i].cortData = new CorTMessage();                                                                    // Allocate memory for the correlation/termination message data
            ifs.read(reinterpret_cast<char *>(&(messages[i].cortData->orderId)), sizeof(messages[i].cortData->orderId)); // Read orderId (8 bytes)
            break;
        case 'M':                                                                                                              // Modify Message
            messages[i].modifyData = new ModifyMessage();                                                                      // Allocate memory for the modify message data
            ifs.read(reinterpret_cast<char *>(&(messages[i].modifyData->price)), sizeof(messages[i].modifyData->price));       // Read price (8 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].modifyData->quantity)), sizeof(messages[i].modifyData->quantity)); // Read quantity (8 bytes)
            ifs.read(reinterpret_cast<char *>(&(messages[i].modifyData->orderId)), sizeof(messages[i].modifyData->orderId));   // Read orderId (8 bytes)
            break;
        case 'Q':                                                                                                            // Query Message
            messages[i].queryData = new QueryMessage();                                                                      // Allocate memory for the query message data
            ifs.read(reinterpret_cast<char *>(&(messages[i].queryData->symbolId)), sizeof(messages[i].queryData->symbolId)); // Read symbolId (4 bytes)
            break;

        default:
            cerr << "Error while reading the message : unexpected msg type " << int(messages[i].msgType) << " \n";
            exit(1); // Exit the program with an error code if an unexpected message type is encountered
            break;
        }
    }
    return true; // Return true to indicate successful reading of the packet
}

// Function to initialize packets from the list of input file names
void Decoder::initializePackets(vector<string> &filenames)
{
    minTimestamp = std::numeric_limits<uint64_t>::max(); // Initialize minTimestamp to the maximum value of uint64_t
    minTimestampFileIndex = -1;
    for (int i = 0; i < numFiles; i++)
    {
        ifss.emplace_back(filenames[i], ios::binary); // Open the file as a binary input file stream and store it in ifss vector
        if (readNextPacket(ifss[i], packets[i]))      // Read the next packet from the input file
        {
            if (packets[i]->ph.timestamp < minTimestamp) // Check if the packet's timestamp is smaller than the current minimum
            {
                minTimestamp = packets[i]->ph.timestamp; // Update the minimum timestamp
                minTimestampFileIndex = i;               // Update the index of the file with the minimum timestamp
            }
        }
        else
            activeFiles[i] = false; // If reading fails, mark the file as inactive
    }
}

// Function to update packets after processing the current minimum timestamp packet
void Decoder::updatePackets()
{
    if (!readNextPacket(ifss[minTimestampFileIndex], packets[minTimestampFileIndex])) // Read the next packet from the file
    {
        activeFiles[minTimestampFileIndex] = false; // If reading fails, mark the file as inactive
    }

    minTimestamp = std::numeric_limits<uint64_t>::max(); // Reset minTimestamp to the maximum value of uint64_t
    minTimestampFileIndex = -1;                          // Reset minTimestampFileIndex to -1
    for (int i = 0; i < numFiles; i++)                   // Loop through each input file
    {
        if (activeFiles[i] && packets[i]->ph.timestamp < minTimestamp) // Check if the file is active and its timestamp is smaller than the current minimum
        {
            minTimestamp = packets[i]->ph.timestamp; // Update the minimum timestamp
            minTimestampFileIndex = i;               // Update the index of the file with the minimum timestamp
        }
    }
}

// Function to close all input file streams
void Decoder::close()
{
    for (int i = 0; i < numFiles; i++)
    {
        ifss[i].close(); // Close each input file stream
    }
}

Decoder::Decoder(vector<string> &filenames)
{
    numFiles = filenames.size();
    ifss.reserve(numFiles);
    packets.assign(numFiles, nullptr);
    activeFiles.assign(numFiles, true);
    initializePackets(filenames);
}

Decoder::~Decoder()
{
    for (int i = 0; i < numFiles; i++)
    {
        assert(activeFiles[i] == false); // Assert that all files are marked as inactive before destruction
    }
    close(); // Close all input file streams when the Decoder is destroyed
}

// Function to get the minimum timestamp packet and update packets accordingly
Packet *Decoder::giveMinPacket()
{
    if (minTimestampFileIndex == -1) // If no active file has packets left to process, return nullptr
        return nullptr;
    Packet *packet = packets[minTimestampFileIndex]; // Get the packet with the minimum timestamp

    updatePackets(); // Update the packets after processing the current minimum timestamp packet

    return packet; // Return the minimum timestamp packet for processing
}
