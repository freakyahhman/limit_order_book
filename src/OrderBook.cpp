#include "lob/OrderBook.h"
#include <iostream>
#include <algorithm>
#include <cstdint>

namespace lob {
    void OrderBook::addOrder(uint64_t id, Side side, uint64_t price, uint32_t quantity) {

        Order* order = order_pool_.allocate(id, side, price, quantity);

        matchOrder(order);

        if(order->quantity_ > 0) {
            order_map_[id] = order;
            if(side == Side::Buy) {
                bids_[price].addOrder(order);
            }
            else {
                asks_[price].addOrder(order);
            }
        }
        else {
            order_pool_.deallocate(order);
        }
    }

    void OrderBook::matchOrder(Order* order) {
        if(order->side_ == Side::Buy) {
            while(!asks_.empty() && asks_.begin()->first <= order->price_) {
                if(order->quantity_ == 0) break;
                PriceLevel& level = asks_.begin()->second;
                while(level.getHead() != nullptr) {
                    Order* sell = level.getHead();
                    if(sell->quantity_ <= order->quantity_) {
                        order->quantity_ -= sell->quantity_;
                        sell->quantity_ = 0;
                        level.removeOrder(sell);
                        order_map_[sell->id_] = nullptr;
                        order_pool_.deallocate(sell);
                        if(order->quantity_ == 0) break;
                    }
                    else {
                        sell->quantity_ -= order->quantity_;
                        order->quantity_ = 0;
                        break;
                    }
                }
                if(level.getHead() == nullptr) asks_.erase(asks_.begin());
            }
        }
        else if (order->side_ == Side::Sell) {
            while(!bids_.empty() && bids_.begin()->first >= order->price_) {
                
                if(order->quantity_ == 0) break;
                
                PriceLevel& level = bids_.begin()->second;
                
                while(level.getHead() != nullptr) {
                    Order* buy = level.getHead();
                    
                    if(buy->quantity_ <= order->quantity_) {
                        order->quantity_ -= buy->quantity_;
                        buy->quantity_ = 0;
                        
                        level.removeOrder(buy);
                        order_map_[buy->id_] = nullptr;
                        order_pool_.deallocate(buy);
                        
                        if(order->quantity_ == 0) break;
                    }
                    else {
                        buy->quantity_ -= order->quantity_;
                        order->quantity_ = 0;
                        break;
                    }
                }
                
                if(level.getHead() == nullptr) bids_.erase(bids_.begin());
            }
        }
    }

    void OrderBook::cancelOrder(uint64_t id) {
        if(order_map_.find(id) == order_map_.end() || order_map_[id] == nullptr) return;
        Order* order = order_map_[id];
        if(order->side_ == Side::Buy) {
            PriceLevel& level = bids_[order->price_];
            level.removeOrder(order);
            if(level.getHead() == nullptr) {
                bids_.erase(order->price_);
            }
        }
        else {
            PriceLevel& level = asks_[order->price_];
            level.removeOrder(order);
            if(level.getHead() == nullptr) {
                asks_.erase(order->price_);
            }
        }
        order_map_[id] = nullptr;
        order_pool_.deallocate(order);
    }

    void OrderBook::printBook() const {
        std::cout << "Bids:\n";
        for(const auto& p : bids_) {
            std::cout << "Price: " << p.first << '\n';
            std::cout << "__________\n";
            const PriceLevel& level = p.second;
            Order* curr = level.getHead();
            while(curr != nullptr) {
                std::cout << "ID: " << curr->id_ << " | Quantity: " << curr->quantity_ << '\n';
                curr = curr->next;
            }
        }
        std::cout << "____________________\n";
        std::cout << "Asks:\n";
        for(const auto& p : asks_) {
            std::cout << "Price: " << p.first << '\n';
            std::cout << "__________\n";
            const PriceLevel& level = p.second;
            Order* curr = level.getHead();
            while(curr != nullptr) {
                std::cout << "ID: " << curr->id_ << " | Quantity: " << curr->quantity_ << '\n';
                curr = curr->next;
            }
        }
    }
}