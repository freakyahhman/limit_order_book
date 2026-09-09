#include <iostream>
#include "lob/OrderBook.h"

using namespace lob;

int main() {
    std::cout << "🚀 Initializing Limit Order Book...\n\n";
    OrderBook book;

    std::cout << "📈 Populating Asks (Sellers)...\n";
    book.addOrder(1, Side::Sell, 105, 100);
    book.addOrder(2, Side::Sell, 105, 50);  // Same price, tests FIFO queue
    book.addOrder(3, Side::Sell, 106, 200);

    std::cout << "📉 Populating Bids (Buyers)...\n";
    book.addOrder(4, Side::Buy, 103, 150);
    book.addOrder(5, Side::Buy, 102, 100);

    std::cout << "\n📊 Initial Book State:\n";
    book.printBook();

    std::cout << "\n⚡ Aggressive Buy Order (ID 6, Qty 120 @ 105) crosses the spread!\n";
    // Should fully fill ID 1 (100) and partially fill ID 2 (leaving 30)
    book.addOrder(6, Side::Buy, 105, 120); 

    std::cout << "\n📊 Book State After Match:\n";
    book.printBook();

    std::cout << "\n🛑 Canceling Order ID 2 (Remaining 30 Qty)...\n";
    book.cancelOrder(2);

    std::cout << "\n📊 Final Book State:\n";
    book.printBook();

    return 0;
}