// This is the implementation file for the OrderBook class.

#include "../include/orderbook.hpp"

// Function to process a new message and update the order book accordingly
void OrderBook::processNew(NewMessage &nm)
{
    orderIdToOrder[nm.orderId] = new Order(nm); // Create a new Order and add it to orderIdToOrder map
    auto &qc = symbolBooks[nm.symbolId].books[nm.side][nm.price];
    qc.first += nm.quantity; // Update the total quantity at the specific price level
    qc.second++;             // Increment the count of orders at the specific price level
}

// Function to process a correlation or termination message and update the order book accordingly
void OrderBook::processCorT(CorTMessage &cortm)
{
    assert(orderIdToOrder.count(cortm.orderId) != 0);
    Order *order = orderIdToOrder[cortm.orderId]; // Find the order using its orderId
    orderIdToOrder.erase(cortm.orderId);          // Remove the order from the orderIdToOrder map

    assert(symbolBooks.count(order->symbolId) != 0);
    auto &sideBook = symbolBooks[order->symbolId].books[order->side];

    assert(sideBook.count(order->price) != 0);
    sideBook[order->price].first -= order->quantity; // Reduce the total quantity at the specific price level
    sideBook[order->price].second--;                 // Decrement the count of orders at the specific price level

    // If there are no more orders at the specific price level, remove the price level from the book
    if (sideBook[order->price].first == 0)
    {
        assert(sideBook[order->price].second == 0);
        sideBook.erase(order->price);
    }

    free(order); // Free the memory allocated to the order
}

// Function to process a modify message and update the order book accordingly
void OrderBook::processModify(ModifyMessage &mm)
{
    assert(orderIdToOrder.count(mm.orderId) != 0);
    Order *order = orderIdToOrder[mm.orderId]; // Find the order using its orderId
    assert(order->price == mm.price);          // Ensure that the order's price matches the modify message's price
    uint64_t prevQuantity = order->quantity;   // Store the previous quantity for updating the book

    order->quantity = mm.quantity; // Update the order's quantity to the modified quantity

    assert(symbolBooks.count(order->symbolId) != 0 && symbolBooks[order->symbolId].books[order->side].count(order->price) != 0);
    auto &qc = symbolBooks[order->symbolId].books[order->side][order->price];
    qc.first -= prevQuantity;
    qc.first += order->quantity;
}

// Function to process a query message and send the query response to the encoder
void OrderBook::processQuery(QueryMessage &qm, Encoder &encoder)
{
    assert(symbolBooks.count(qm.symbolId) != 0);
    auto &books = symbolBooks[qm.symbolId].books; // Get the books for the queried symbol

    QueryResponse queryResponse;
    if (!books[0].empty())
    {
        // If there are bids, find the maximum price and its corresponding quantity and count
        queryResponse.bidPrice = books[0].rbegin()->first;
        queryResponse.bidQty = books[0].rbegin()->second.first;
        queryResponse.bidCount = books[0].rbegin()->second.second;
    }

    if (!books[1].empty())
    {
        // If there are asks, find the minimum price and its corresponding quantity and count
        queryResponse.askPrice = books[1].begin()->first;
        queryResponse.askQty = books[1].begin()->second.first;
        queryResponse.askCount = books[1].begin()->second.second;
    }

    printQueryResponse(queryResponse, encoder);
}

// Function to print the query response through the encoder
void OrderBook::printQueryResponse(QueryResponse &qr, Encoder &encoder)
{
    encoder.printQueryResponse(qr);
}

// Function to process a generic message and call the appropriate handler based on the message type
void OrderBook::processMessage(Message &message, Encoder &encoder)
{
    switch (message.msgType)
    {
    case 'N':
        processNew(*(message.newData));
        break;
    case 'C':
    case 'T':
        processCorT(*(message.cortData));
        break;
    case 'M':
        processModify(*(message.modifyData));
        break;
    case 'Q':
        processQuery(*(message.queryData), encoder);
        break;
    default:
        assert(false);
        break;
    }
}
