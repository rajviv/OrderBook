#include "include/utils.hpp"
#include <filesystem>
namespace fs = std::filesystem;

void convert(string filename, string outputFilename) // Convert the file (.txt) to corresponding (.dat) file
{
    ifstream inputFile(filename);
    ofstream outputFile(outputFilename, ios::binary);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    PacketHeader ph;
    while (inputFile >> ph.timestamp >> ph.numMessages) // Reading till there are still packets in the file left.
    {
        outputFile.write(reinterpret_cast<const char *>(&(ph.timestamp)), sizeof(ph.timestamp)); // Writing to .dat file
        outputFile.write(reinterpret_cast<const char *>(&(ph.numMessages)), sizeof(ph.numMessages));

        for (int i = 0; i < ph.numMessages; i++) // Going through all messages and writing them to (.dat) file
        {
            Message message;

            inputFile >> message.msgType;
            outputFile.write(reinterpret_cast<const char *>(&(message.msgType)), sizeof(message.msgType));

            switch (message.msgType)
            {
            case 'N':
                message.newData = new NewMessage();
                inputFile >> message.newData->price;
                inputFile >> message.newData->quantity;
                inputFile >> message.newData->symbolId;
                inputFile >> message.newData->orderId;
                inputFile >> message.newData->side;
                message.newData->s = static_cast<uint8_t>(message.newData->side);

                outputFile.write(reinterpret_cast<const char *>(&(message.newData->price)), sizeof(message.newData->price));
                outputFile.write(reinterpret_cast<const char *>(&(message.newData->quantity)), sizeof(message.newData->quantity));
                outputFile.write(reinterpret_cast<const char *>(&(message.newData->symbolId)), sizeof(message.newData->symbolId));
                outputFile.write(reinterpret_cast<const char *>(&(message.newData->orderId)), sizeof(message.newData->orderId));
                outputFile.write(reinterpret_cast<const char *>(&(message.newData->s)), sizeof(message.newData->s));

                free(message.newData);
                break;
            case 'C':
            case 'T':
                message.cortData = new CorTMessage();
                inputFile >> message.cortData->orderId;

                outputFile.write(reinterpret_cast<const char *>(&(message.cortData->orderId)), sizeof(message.cortData->orderId));

                free(message.cortData);
                break;
            case 'M':
                message.modifyData = new ModifyMessage();
                inputFile >> message.modifyData->price;
                inputFile >> message.modifyData->quantity;
                inputFile >> message.modifyData->orderId;

                outputFile.write(reinterpret_cast<const char *>(&(message.modifyData->price)), sizeof(message.modifyData->price));
                outputFile.write(reinterpret_cast<const char *>(&(message.modifyData->quantity)), sizeof(message.modifyData->quantity));
                outputFile.write(reinterpret_cast<const char *>(&(message.modifyData->orderId)), sizeof(message.modifyData->orderId));

                free(message.modifyData);
                break;
            case 'Q':
                message.queryData = new QueryMessage();
                inputFile >> message.queryData->symbolId;

                outputFile.write(reinterpret_cast<const char *>(&(message.queryData->symbolId)), sizeof(message.queryData->symbolId));

                free(message.queryData);
                break;
            default:
                break;
            }
        }
    }
    inputFile.close();
    outputFile.close();
}

string changeFileExtension(const string &filename, const string &newExtension)
{
    size_t dotPos = filename.find_last_of(".");
    if (dotPos != string::npos)
    {
        string newFilename = filename.substr(0, dotPos + 1) + newExtension;
        return newFilename;
    }
    else
    {
        return filename;
    }
}

int main(int argc, char **argv)
{
    string inputFilePath;
    if (argc != 2)
    {
        cout << "Usage: ./orderbook <folder containing data files, eg. tests/test1>\n";
        exit(1);
    }
    else if (argc == 2)
    {
        inputFilePath = string(argv[1]);
    }

    try
    {
        vector<string> filenames;
        for (const auto &entry : fs::directory_iterator(inputFilePath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".txt")
            {
                string inputFile = entry.path();
                filenames.push_back(inputFile);
            }
        }

        for (auto inputFile : filenames)
        {
            string outputFile = changeFileExtension(inputFile, "dat");
            convert(inputFile, outputFile);
        }
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}