#include <benchmark/benchmark.h>
#include "lob/OrderBook.h"

using namespace lob;

static void BM_AddOrder(benchmark::State& state) {
    OrderBook book;
    uint64_t id = 1;
    
    // The framework runs this loop as many times as needed to get a stable average
    for (auto _ : state) {
        book.addOrder(id++, Side::Buy, 100, 10);
    }
}
BENCHMARK(BM_AddOrder);

static void BM_MatchOrder(benchmark::State& state) {
    OrderBook book;
    uint64_t id = 1;
    
    // Pre-fill the book with resting liquidity
    for(int i = 0; i < 1000; i++) {
        book.addOrder(id++, Side::Sell, 105, 100);
    }

    // Measure strictly how long it takes to cross the spread and execute a trade
    for (auto _ : state) {
        book.addOrder(id++, Side::Buy, 105, 100); 
    }
}
BENCHMARK(BM_MatchOrder);

BENCHMARK_MAIN();