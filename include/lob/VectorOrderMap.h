#pragma once
#include <vector>
#include <cstdint>
#include "Order.h"

namespace lob {

class VectorOrderMap {
private:
    std::vector<Order*> slots_;

public:
    VectorOrderMap(size_t capacity = 1048576) {
        slots_.resize(capacity, nullptr);
    }

    // Direct memory access bypassing all hashing overhead
    Order*& operator[](uint64_t id) {
        return slots_[id];
    }

    // Support both get() and find()
    Order* get(uint64_t id) const {
        return slots_[id];
    }

    Order* find(uint64_t id) const {
        return slots_[id];
    }

    Order* end() const {
        return nullptr;
    }

    void erase(uint64_t id) {
        slots_[id] = nullptr;
    }
};

}