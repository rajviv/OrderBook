#include "../include/utils.hpp"

// This is for just printing and debugging purpose.
map<char, string> msgType_verbose = {
    {'N', "New"},
    {'C', "Cancel"},
    {'T', "Trade"},
    {'M', "Modify"},
    {'Q', "Query"}};

// Following are the stream insertion overloading function for various structs defined in utils.hpp, these are used for debugging purpuses.

ostream &operator<<(ostream &os, const PacketHeader &ph)
{
    os << "[PacketHeader |timestamp:" << ph.timestamp << "|numMessages:" << ph.numMessages << "]\n";
    return os;
}

ostream &operator<<(ostream &os, const NewMessage &nm)
{
    os << "price:" << nm.price << "|quantity:" << nm.quantity << "|symbolid:" << nm.symbolId << "|orderid:" << nm.orderId << "|side:" << nm.side;
    return os;
}

ostream &operator<<(ostream &os, const CorTMessage &ctm)
{
    os << "orderid:" << ctm.orderId;
    return os;
}

ostream &operator<<(ostream &os, const ModifyMessage &mm)
{
    os << "price:" << mm.price << "|quantity:" << mm.quantity << "|orderid:" << mm.orderId;
    return os;
}

ostream &operator<<(ostream &os, const QueryMessage &qm)
{
    os << "symbolid:" << qm.symbolId;
    return os;
}

ostream &operator<<(ostream &os, const Message &m)
{
    os << "[" << msgType_verbose[m.msgType] << "|msgType:" << m.msgType << "|";
    switch (m.msgType)
    {
    case 'N':
        os << *(m.newData);
        break;
    case 'C':
    case 'T':
        os << *(m.cortData);
        break;
    case 'M':
        os << *(m.modifyData);
        break;
    case 'Q':
        os << *(m.queryData);
        break;
    }
    os << "]\n";
    return os;
}

ostream &operator<<(ostream &os, const QueryResponse &qr)
{
    os << "[Response:|bidprice:" << qr.bidPrice << "|bidqty:" << qr.bidQty << "|bidcount:" << qr.bidCount;
    os << "|askprice:" << qr.askPrice << "|askqty:" << qr.askQty << "|askcount:" << qr.askCount << "]\n";
    return os;
}

// This is for recursively freeing a struct Packet
void freePacket(Packet *packet)
{
    for (int i = 0; i < packet->messages.size(); i++)
    {
        switch (packet->messages[i].msgType)
        {
        case 'N':
            free(packet->messages[i].newData);
            break;
        case 'C':
        case 'T':
            free(packet->messages[i].cortData);
            break;
        case 'M':
            free(packet->messages[i].modifyData);
            break;
        case 'Q':
            free(packet->messages[i].queryData);
            break;
        }
    }
    free(packet);
}