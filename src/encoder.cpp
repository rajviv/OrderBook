#include "../include/encoder.hpp"

void Encoder::printValue(uint64_t x)
{
    ofs.write(reinterpret_cast<const char *>(&x), sizeof(x));
}

void Encoder::printValue(uint32_t x)
{
    ofs.write(reinterpret_cast<const char *>(&x), sizeof(x));
}

Encoder::Encoder(const string filename)
{
    ofs.open(filename, ios::binary);
}

Encoder::~Encoder()
{
    ofs.close();
}

void Encoder::printQueryResponse(const QueryResponse &qr)
{
    printValue(qr.bidPrice);
    printValue(qr.bidQty);
    printValue(qr.bidCount);
    printValue(qr.askPrice);
    printValue(qr.askQty);
    printValue(qr.askCount);
}
