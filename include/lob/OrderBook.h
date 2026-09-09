#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include "Order.h"
#include "PriceLevel.h"
#include "MemoryPool.h"

namespace lob {

class OrderBook {
public:
    OrderBook() = default;
    ~OrderBook() = default;

    /// @brief Submits a new order to the exchange. It will immediately execute against resting liquidity if possible, 
    /// or park the remaining quantity in the order book.
    /// @param id uint64_t -> id of the order.
    /// @param side Side::Buy or Side::Sell.
    /// @param price uint64_t -> price of the order.
    /// @param quantity uint32_t -> quantity of the order.
    void addOrder(uint64_t id, Side side, uint64_t price, uint32_t quantity);

    /// @brief Withdraws a resting limit order from the exchange, completely terminating its active status.
    /// @param id uint64_t id of the order.
    void cancelOrder(uint64_t id);

    void printBook() const;

private:
    std::unordered_map<uint64_t, Order*> order_map_;
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids_;
    std::map<uint64_t, PriceLevel, std::less<uint64_t>> asks_;
    MemoryPool order_pool_;

    /// Evaluates an aggressive incoming order against resting limit orders on the opposite side to execute trades.
    void matchOrder(Order* order);

};











}