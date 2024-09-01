#define __ENCODER__

#ifndef __UTILS__
#include "utils.hpp"
#endif

class Encoder
{
private:
    ofstream ofs;
    void printValue(uint64_t x);
    void printValue(uint32_t x);

public:
    Encoder(const string filename);
    ~Encoder();
    void printQueryResponse(const QueryResponse &qr);
};