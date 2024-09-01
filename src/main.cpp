#include "../include/utils.hpp"
#include "../include/encoder.hpp"
#include "../include/decoder.hpp"
#include "../include/orderbook.hpp"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem; // Namespace alias for std::filesystem

int main(int argc, char **argv)
{
    string folderPath, outputFilename = "output.dat";
    if (argc == 1 || argc >= 4)
    {
        cout << "Usage: ./orderbook <folder containing data files, eg. tmp/thdata> [output filename, default output.dat]\n";
        exit(1);
    }
    else if (argc == 2)
    {
        folderPath = string(argv[1]);
    }
    else if (argc == 3)
    {
        folderPath = string(argv[1]);
        outputFilename = string(argv[2]);
    }

    vector<string> filenames; // Initializing a vector to store the names of data files in the folder
    try
    {
        // Looping through the directory entries to find data files with .dat extension
        for (const auto &entry : fs::directory_iterator(folderPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".dat")
            {
                filenames.push_back(entry.path());
            }
        }
        if (filenames.size() == 0)
        {
            std::cerr << "Error: No .dat files on this path" << endl; // Error message when no .dat files found
            return 1;                                                 // Exiting the program with an error code
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl; // Error message when an exception occurs during directory iteration
        return 1;                                        // Exiting the program with an error code
    }

    Decoder decoder(filenames);
    outputFilename = "output/" + outputFilename;
    Encoder encoder(outputFilename);
    OrderBook orderBook;

    Packet *packet;
    while ((packet = decoder.giveMinPacket()) != nullptr) // Looping through packets from the decoder
    {
        int numMessages = packet->messages.size();
        for (int i = 0; i < numMessages; i++) // Looping through the messages in the packet
        {
            orderBook.processMessage(packet->messages[i], encoder);
        }
        free(packet); // Freeing the memory allocated to the packet
    }

    return 0;
}
