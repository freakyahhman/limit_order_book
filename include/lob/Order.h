#pragma once

#include <cstdint>


namespace lob {

enum class Side : uint8_t {
    Buy = 0,
    Sell = 1
};


struct Order {
    uint64_t id_;
    uint64_t price_;
    uint32_t quantity_;
    Side side_;
    
    Order* next;
    Order* prev;
    
    /// @brief Initializes a new market participant request.
    /// @param id uint64_t -> id of the order.
    /// @param side Side::Buy or Side::Sell.
    /// @param price uint64_t -> the price of the order.
    /// @param quantity uint32_t -> quantity of the order.
    Order(uint64_t id, Side side, uint64_t price, uint32_t quantity)
        : id_(id), price_(price), quantity_(quantity), side_(side), next(nullptr), prev(nullptr) {}
        
    ~Order() = default;
};

}