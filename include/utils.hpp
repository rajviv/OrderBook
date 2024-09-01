// This is the header file for a C++ program related to handling packet and message data.

#define __UTILS__

// Including necessary libraries
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cassert>
#include <limits>
using namespace std;

// Definition of various structs used in the program

// Struct to represent the header of a packet
typedef struct PacketHeader
{
    uint64_t timestamp;
    uint32_t numMessages;
} PacketHeader;

// Struct to represent a new message
typedef struct NewMessage
{
    uint64_t price;
    uint64_t quantity;
    uint32_t symbolId;
    uint64_t orderId;
    uint8_t s;
    int side;
} NewMessage;

// Struct to represent a cancel or trade message
typedef struct CorTMessage
{
    uint64_t orderId;
} CorTMessage;

// Struct to represent a modify message
typedef struct ModifyMessage
{
    uint64_t price;
    uint64_t quantity;
    uint64_t orderId;
} ModifyMessage;

// Struct to represent a query message
typedef struct QueryMessage
{
    uint32_t symbolId;
} QueryMessage;

// Struct to represent a generic message that can be of different types
typedef struct Message
{
    char msgType;
    NewMessage *newData;
    CorTMessage *cortData;
    ModifyMessage *modifyData;
    QueryMessage *queryData;
} Message;

// Struct to represent a packet containing header and a vector of messages
typedef struct Packet
{
    PacketHeader ph;
    vector<Message> messages;
} Packet;

// Struct to represent an order, which is derived from the new message
typedef struct Order
{
    uint64_t price;
    uint64_t quantity;
    uint32_t symbolId;
    uint64_t orderId;
    int side;
    Order(NewMessage &nm) : price(nm.price), quantity(nm.quantity), symbolId(nm.symbolId), orderId(nm.orderId), side(nm.side) {}
} Order;

// Struct to represent a symbol book, which contains a map for bid and ask information
typedef struct SymbolBook
{
    map<uint64_t, pair<uint64_t, uint32_t>> books[2]; // An array of maps representing bid and ask books
} SymbolBook;

// Struct to represent a response to a query message, containing bid and ask information
typedef struct QueryResponse
{
    uint64_t bidPrice;
    uint64_t bidQty;
    uint32_t bidCount;
    uint64_t askPrice;
    uint64_t askQty;
    uint32_t askCount;
    QueryResponse()
    {
        bidPrice = bidQty = bidCount = askPrice = askQty = askCount = 0;
    }
} QueryResponse;

// Overloading the << operator for output stream to print various structs
ostream &operator<<(ostream &os, const PacketHeader &ph);
ostream &operator<<(ostream &os, const NewMessage &nm);
ostream &operator<<(ostream &os, const CorTMessage &ctm);
ostream &operator<<(ostream &os, const ModifyMessage &mm);
ostream &operator<<(ostream &os, const QueryMessage &qm);
ostream &operator<<(ostream &os, const Message &m);
void freePacket(Packet *packet);                           // Function to free memory allocated to a packet and its contained data
ostream &operator<<(ostream &os, const QueryResponse &qr); // Overloading the << operator for output stream to print QueryResponse struct
