#define __DECODER__

#ifndef __UTILS__
#include "utils.hpp"
#endif

class Decoder
{
private:
    int numFiles;
    vector<ifstream> ifss;      // Filestreams for the input data files.
    vector<Packet *> packets;   // Vector of next unprocessed packets of all the data files (one for each).
    vector<bool> activeFiles;   // TO keep record of data file completion.
    uint64_t minTimestamp;
    int minTimestampFileIndex;

    bool readNextPacket(ifstream &ifs, Packet *&packet);
    void initializePackets(vector<string> &filenames);
    void updatePackets();
    void close();

public:
    Decoder(vector<string> &filenames);
    ~Decoder();
    Packet *giveMinPacket();
};
