# ⚡ High-Performance Limit Order Book (LOB)

A low-latency, strictly Price-Time priority matching engine written in C++17. Designed for high-frequency trading (HFT) environments, this engine avoids operating system latency spikes by utilizing custom memory management and cache-friendly data structures.

## 🏗️ Architecture

The matching engine separates concerns into three distinct layers to maximize CPU cache retention and minimize algorithmic complexity:

*   **Memory Pool (`MemoryPool.h`):** A pre-allocated object pool that eliminates OS-level `new` and `delete` calls on the hot path, achieving strictly `O(1)` zero-allocation order creation and destruction.
*   **Price Levels (`PriceLevel.h`):** Implemented as intrusive doubly-linked lists. This guarantees `O(1)` order enqueueing, dequeueing, and cancellations without pointer fragmentation.
*   **Order Book (`OrderBook.h`):** Utilizes Red-Black Trees (`std::map`) for `O(log P)` price-level traversal, mapping price points directly to their corresponding queues.

## ⏱️ Performance Benchmarks

Micro-benchmarking is powered by **Google Benchmark** to ensure rigorous, nanosecond-precision profiling. 

*Currently measured natively on an Apple M4 (10-core) compiled with `-O3` Release optimizations:*

| Operation | Latency | Complexity |
| :--- | :--- | :--- |
| **Order Insertion (Resting)** | ~32 ns | `O(1)` ID lookup, `O(log P)` Price Level |
| **Order Matching (Aggressive)** | ~30 ns | `O(log P)` traversal, `O(1)` execution |

*Current throughput supports approximately **30+ million orders per second**.*

## 🚀 Build Instructions

This project requires **CMake** and a modern C++ compiler.

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./lob_benchmark