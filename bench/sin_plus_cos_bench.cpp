#include <benchmark/benchmark.h>

#include "codeman-wrapper.hpp"

using namespace chai::interpreter;

static void initSinPlusCos(CodeManWrapper &codeman);

static void BM_SinCos(benchmark::State &state) {
    // Perform setup here
    CodeManWrapper wrapper{};
    initSinPlusCos(wrapper);
    for (auto _ : state) {
        chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024 * 256);;
        Executor executor{&wrapper.manager_, buffer_};
        executor.run();
    }
}
BENCHMARK(BM_SinCos);

static void initSinPlusCos(CodeManWrapper &codeman) {
    const RegisterId r1 = 1;
    const RegisterId r2 = 2;
    const RegisterId r3 = 3;
    const RegisterId r4 = 4;

    // r1 = 314, r2 = 271, r3 = 60
    codeman.loadWithConst(Ldiaf, 314.0);
    codeman.load(Star, r1);
    codeman.loadWithConst(Ldiaf, 271.0);
    codeman.load(Star, r2);
    codeman.loadWithConst(Ldiaf, 60.0);
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
    codeman.update();
}
