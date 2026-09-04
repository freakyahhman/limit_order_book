#pragma once

#include <cstdint>
#include "Order.h"

namespace lob {

class PriceLevel {
private:
    Order* list_head_;
    Order* list_tail_;
    uint64_t total_volume_;

public:
    /// @brief initializes a queue of orders that have the same price.
    PriceLevel() 
        : list_head_(nullptr), list_tail_(nullptr), total_volume_(0) {}
    
    ~PriceLevel() = default;

    /// @brief access the first element of the queue.
    /// @return Order* -> the head of the doubly linked list.
    Order* getHead() const { return list_head_; }

    /// @brief access the last element of the queue.
    /// @return Order* -> the tail of the doubly linked list.
    Order* getTail() const { return list_tail_; }

    /// @brief total volume of this price.
    /// @return uint64_t.
    uint64_t getTotalVolume() const { return total_volume_; }

    /// @brief checks if the queue is empty.
    /// @return bool.
    bool isEmpty() const { return list_head_ == nullptr; }

    /// @brief adds an order to the queue.
    /// @param order Order*
    void addOrder(Order* order) {
        total_volume_ += order->quantity_;

        if (list_head_ == nullptr) {
            list_head_ = list_tail_ = order;
            return;
        }
        
        list_tail_->next = order;
        order->prev = list_tail_;
        list_tail_ = order;
    }

    /// @brief removes an order from the queue.
    /// @param order Order*.
    void removeOrder(Order* order) {
        if (list_head_ == nullptr || order == nullptr) return;

        total_volume_ -= order->quantity_;

        // 1. Single node in the list
        if (list_head_ == list_tail_ && list_head_ == order) {
            list_head_ = list_tail_ = nullptr;
        } 
        else {
            // 2. Wiring the previous node
            if (order->prev != nullptr) {
                order->prev->next = order->next;
            } else {
                // If there's no previous node, this order was the head
                list_head_ = order->next;
            }

            // 3. Wiring the next node
            if (order->next != nullptr) {
                order->next->prev = order->prev;
            } else {
                // If there's no next node, this order was the tail
                list_tail_ = order->prev;
            }
        }

        // 4. Sever ties completely to avoid dangling pointers
        order->next = nullptr;
        order->prev = nullptr;
    }
};

}