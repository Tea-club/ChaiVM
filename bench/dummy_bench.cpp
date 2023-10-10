#include <benchmark/benchmark.h>
#include "ChaiVM/interpreter/executor.hpp"


static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {

        // This code gets timed
        chai::interpreter::CodeManager codeManager{};
        chai::interpreter::Executor executor{&codeManager};

    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
