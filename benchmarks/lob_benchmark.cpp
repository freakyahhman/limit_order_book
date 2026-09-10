#include <benchmark/benchmark.h>
#include "lob/OrderBook.h"

using namespace lob;

static void BM_AddOrder(benchmark::State& state) {
    OrderBook book;
    uint64_t id = 1;
    
    // Benchmark only 500k resting orders to fit inside our 1M pre-allocated memory pool
    for (auto _ : state) {
        book.addOrder(id++, Side::Buy, 100, 10);
    }
}
// Force exactly 500,000 iterations to prevent overflowing our fixed-size arrays
BENCHMARK(BM_AddOrder)->Iterations(500000);

static void BM_MatchOrder(benchmark::State& state) {
    OrderBook book;
    uint64_t id = 1;
    
    // Pre-fill the book with EXACTLY 500,000 resting sell orders
    // so every single benchmark iteration executes a successful trade.
    for(int i = 0; i < 500000; i++) {
        book.addOrder(id++, Side::Sell, 105, 100);
    }

    // Measure strictly how long it takes to cross the spread and execute the trade
    for (auto _ : state) {
        book.addOrder(id++, Side::Buy, 105, 100); 
    }
}
BENCHMARK(BM_MatchOrder)->Iterations(500000);

BENCHMARK_MAIN();