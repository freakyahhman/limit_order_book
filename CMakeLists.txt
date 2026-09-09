cmake_minimum_required(VERSION 3.20)
project(LimitOrderBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3")

include_directories(include)
add_executable(lob_engine src/main.cpp src/OrderBook.cpp)

include(FetchContent)

# 🛑 Fix: Disable Google Benchmark's internal tests to prevent CMake version crashes
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install" FORCE)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
FetchContent_MakeAvailable(googlebenchmark)

# Create the benchmark executable
add_executable(lob_benchmark benchmarks/lob_benchmark.cpp src/OrderBook.cpp)
target_link_libraries(lob_benchmark benchmark::benchmark)
target_include_directories(lob_benchmark PRIVATE include)