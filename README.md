# ⚡ High-Performance Limit Order Book (LOB)

A low-latency, strictly Price-Time priority matching engine written in C++17. Designed for high-frequency trading (HFT) environments, this engine avoids operating system latency spikes by utilizing custom memory management and cache-friendly data structures.

## 🏗️ Architecture

The matching engine separates concerns into three distinct layers to maximize CPU cache retention and minimize algorithmic complexity:

*   **Memory Pool (`MemoryPool.h`):** A pre-allocated object pool that eliminates OS-level `new` and `delete` calls on the hot path, achieving strictly `O(1)` zero-allocation order creation and destruction.
*   **Price Levels (`PriceLevel.h`):** Implemented as intrusive doubly-linked lists. This guarantees `O(1)` order enqueueing, dequeueing, and cancellations without pointer fragmentation.
*   **Order Book (`OrderBook.h`):** Utilizes Red-Black Trees (`std::map`) for `O(log P)` price-level traversal, mapping price points directly to their corresponding queues.

## ⏱️ The Path to Single-Digit Latency

Micro-benchmarking is powered by **Google Benchmark** to ensure rigorous, nanosecond-precision profiling. The evolution of the engine's `order_id` lookup architecture demonstrates the critical impact of cache locality and bypassing standard library overhead.

*Measurements taken natively on an Apple M4 (10-core) compiled with `-O3` Release optimizations:*

| Architecture | Order Insertion | Order Matching | Engineering Notes |
| :--- | :--- | :--- | :--- |
| **1. `std::unordered_map`** | ~32.7 ns | ~30.4 ns | Heavy cache misses from pointer chasing; hidden heap allocations for collision nodes. |
| **2. Custom `FlatOrderMap`** | ~22.1 ns | ~14.5 ns | Open-addressing with linear probing. Eliminated heap allocations and improved L1 cache pre-fetching. |
| **3. `VectorOrderMap`** | **~5.0 ns** | **~7.5 ns** | Pre-allocated direct array access. Bypassed hashing entirely for pure $O(1)$ memory addressing. |

*Current throughput supports over **130+ million orders per second** on a single thread.*

## 🚀 Build Instructions

This project requires **CMake** and a modern C++ compiler.

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./lob_benchmark