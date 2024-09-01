#define __ORDERBOOK__

#ifndef __UTILS__
#include "utils.hpp"
#endif

#ifndef __ENCODER__
#include "encoder.hpp"
#endif

class OrderBook
{
private:
    unordered_map<uint64_t, Order *> orderIdToOrder; // This is a map to keep record of the orderid and corresponding orders
    unordered_map<uint32_t, SymbolBook> symbolBooks; // This is map for symbolId, their buy-sell books.

    void processNew(NewMessage &nm);
    void processCorT(CorTMessage &cortm);
    void processModify(ModifyMessage &mm);
    void processQuery(QueryMessage &qm, Encoder &encoder);
    void printQueryResponse(QueryResponse &qr, Encoder &encoder);

public:
    void processMessage(Message &message, Encoder &encoder);
};
