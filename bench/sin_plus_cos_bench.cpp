#include <benchmark/benchmark.h>

#include "codeman-wrapper.hpp"

using namespace chai::interpreter;

static void initSinPlusCos(CodeManWrapper &codeman);

static void BM_SinCos(benchmark::State &state) {
    // Perform setup here
    CodeManWrapper wrapper{};
    initSinPlusCos(wrapper);
    Executor executor{&wrapper.manager_};
    for (auto _ : state) {
        executor.run();
        executor.restart();
    }
}
BENCHMARK(BM_SinCos);

static void initSinPlusCos(CodeManWrapper &codeman) {
    const RegisterId r1 = 1;
    const RegisterId r2 = 2;
    const RegisterId r3 = 3;
    const RegisterId r4 = 4;

    assert(sizeof(Immidiate) == sizeof(float));
    // r1 = 314, r2 = 271, r3 = 60
    codeman.loadi(Ldiaf, std::bit_cast<Immidiate>(314.0f));
    codeman.load(Star, r1);
    codeman.loadi(Ldiaf, std::bit_cast<Immidiate>(271.0f));
    codeman.load(Star, r2);
    codeman.loadi(Ldiaf, std::bit_cast<Immidiate>(60.0f));
    codeman.load(Star, r3);

    // r4 = sin(r3) * r1
    codeman.load(Ldra, r3);
    codeman.load(IcSin);
    codeman.load(Mulf, r1);
    codeman.load(Star, r4);

    // acc = cos(r3) * r2
    codeman.load(Ldra, r3);
    codeman.load(IcCos);
    codeman.load(Mulf, r2);

    // acc += r4
    codeman.load(Addf, r4);
    codeman.load(Ret);
}
