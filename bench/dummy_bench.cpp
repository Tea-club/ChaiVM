#include "ChaiVM/interpreter/executor.hpp"
#include <benchmark/benchmark.h>

static void BM_SomeFunction(benchmark::State &state) {
    // Perform setup here
    for (auto _ : state) {

        // This code gets timed
        chai::interpreter::CodeManager codeManager{};
        chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024 * 256);;
        chai::interpreter::Executor executor{&codeManager, buffer_};
    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
