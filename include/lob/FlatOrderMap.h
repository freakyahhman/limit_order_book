#pragma once
#include <cstdint>
#include <vector>
#include "Order.h"

namespace lob {

class FlatOrderMap {
public:
    struct Slot {
        uint64_t first; 
        Order* second;
    };

private:
    std::vector<Slot> slots_;
    size_t capacity_;
    size_t mask_;
    
    // Internal markers
    Order* const TOMBSTONE = reinterpret_cast<Order*>(static_cast<uintptr_t>(-1));
    const uint64_t EMPTY_KEY = static_cast<uint64_t>(-1);

    // Fast bitwise hash (SplitMix64 variant)
    size_t getIndex(uint64_t key) const {
        uint64_t x = key;
        x ^= x >> 30;
        x *= 0xbf58476d1ce4e5b9ull;
        x ^= x >> 27;
        x *= 0x94d049bb133111ebull;
        x ^= x >> 31;
        return x & mask_; 
    }

public:
    FlatOrderMap(size_t capacity = 1048576) : capacity_(capacity), mask_(capacity - 1) {
        slots_.resize(capacity_, {EMPTY_KEY, nullptr});
    }

    // Overloaded bracket operator
    // Usage: order_map_[id] = order;
    Order*& operator[](uint64_t id) {
        size_t idx = getIndex(id);
        size_t first_tombstone = static_cast<size_t>(-1);
        
        while (slots_[idx].first != EMPTY_KEY) {
            if (slots_[idx].first == id && slots_[idx].second != TOMBSTONE) {
                return slots_[idx].second; // Return reference to existing value
            }
            if (slots_[idx].second == TOMBSTONE && first_tombstone == static_cast<size_t>(-1)) {
                first_tombstone = idx; // Cache the first available deleted slot
            }
            idx = (idx + 1) & mask_;
        }
        
        // Not found, insert new (fills tombstone if we passed one, else fills empty slot)
        size_t insert_idx = (first_tombstone != static_cast<size_t>(-1)) ? first_tombstone : idx;
        slots_[insert_idx].first = id;
        slots_[insert_idx].second = nullptr; // Default initialize just like standard library
        return slots_[insert_idx].second;
    }

    // API 2: find()
    // Usage: auto it = order_map_.find(id);
    Slot* find(uint64_t id) {
        size_t idx = getIndex(id);
        
        while (slots_[idx].first != EMPTY_KEY) {
            if (slots_[idx].first == id && slots_[idx].second != TOMBSTONE) {
                return &slots_[idx]; // Returns pointer (acts as iterator)
            }
            idx = (idx + 1) & mask_;
        }
        return end();
    }

    // API 3: end()
    // Usage: if (it != order_map_.end()) { ... }
    Slot* end() {
        return nullptr;
    }

    // API 4: erase()
    // Usage: order_map_.erase(id);
    void erase(uint64_t id) {
        size_t idx = getIndex(id);
        
        while (slots_[idx].first != EMPTY_KEY) {
            if (slots_[idx].first == id && slots_[idx].second != TOMBSTONE) {
                slots_[idx].second = TOMBSTONE; // Mark deleted without breaking probing chain
                return;
            }
            idx = (idx + 1) & mask_;
        }
    }
};

} // namespace lob