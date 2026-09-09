#pragma once
#include <vector>
#include "Order.h"

namespace lob {

class MemoryPool {
private:
    std::vector<Order*> free_list_;

public:
    /// @brief Pre-allocates memory for Orders to prevent OS heap allocations during live trading.
    /// @param capacity The initial number of orders to pre-allocate.
    MemoryPool(size_t capacity = 1000000) {
        free_list_.reserve(capacity);
        for (size_t i = 0; i < capacity; ++i) {
            // Allocate on the OS heap exactly once during system startup
            free_list_.push_back(new Order(0, Side::Buy, 0, 0));
        }
    }

    ~MemoryPool() {
        for (Order* order : free_list_) {
            delete order;
        }
    }

    /// @brief Retrieves a recycled Order object in O(1) time.
    Order* allocate(uint64_t id, Side side, uint64_t price, uint32_t quantity) {
        if (free_list_.empty()) {
            // Slow-path fallback if the engine experiences unprecedented volume
            return new Order(id, side, price, quantity); 
        }
        
        // Fast-path O(1) retrieval
        Order* order = free_list_.back();
        free_list_.pop_back();
        
        // Re-initialize the dirty memory block with new data
        order->id_ = id;
        order->side_ = side;
        order->price_ = price;
        order->quantity_ = quantity;
        order->next = nullptr;
        order->prev = nullptr;
        
        return order;
    }

    /// @brief Returns an exhausted Order object back to the pool in O(1) time.
    void deallocate(Order* order) {
        free_list_.push_back(order);
    }
};

}