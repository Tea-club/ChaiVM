#include <benchmark/benchmark.h>

#include "codeman-wrapper.hpp"

using namespace chai::interpreter;

static void initSquareEquatino(CodeManWrapper &codeman);

static void BM_SquareEquation(benchmark::State &state) {
    // Perform setup here
    CodeManWrapper wrapper{};
    initSquareEquatino(wrapper);

    for (auto _ : state) {
        chai::memory::LinearBuffer buffer_ =
            chai::memory::LinearBuffer(1024 * 256);
        ;
        Executor executor{&wrapper.manager_, buffer_};
        executor.run();
    }
}
// Register the function as a benchmark
// @todo #32:60min Measure mips here
BENCHMARK(BM_SquareEquation);

static void initSquareEquatino(CodeManWrapper &codeman) {
    const RegisterId r1 = 1;
    const RegisterId r2 = 2;
    const RegisterId r3 = 3;
    const RegisterId r4 = 4;
    const RegisterId r5 = 5;
    const RegisterId r6 = 6;
    const RegisterId r7 = 7;
    const RegisterId r8 = 8;
    const RegisterId r9 = 9;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
    const RegisterId r10 = 10;
#pragma GCC diagnostic pop
    const RegisterId r11 = 11;

    // r1 = 1.0, r2 = -5.0, r3 = 6.0
    codeman.loadWithConst(Ldiaf, 1.0);
    codeman.load(Star, r1);
    codeman.loadWithConst(Ldiaf, -5.0);
    codeman.load(Star, r2);
    codeman.loadWithConst(Ldiaf, +6.0);
    codeman.load(Star, r3);

    // r4 = -4*r1*r3
    codeman.loadWithConst(Ldiaf, -4.0);
    codeman.load(Mulf, r1);
    codeman.load(Mulf, r3);
    codeman.load(Star, r4);

    // r5 = b * b
    codeman.load(Ldra, r2);
    codeman.load(Mulf, r2);
    codeman.load(Star, r5);

    // r6 = r5 + r4
    codeman.load(Ldra, r5);
    codeman.load(Addf, r4);
    codeman.load(Star, r6);

    // r6 = sqrt(r6)
    codeman.load(Ldra, r6);
    codeman.load(IcSqrt);
    codeman.load(Star, r6);

    // r7 = 2a
    codeman.load(Ldra, r1);
    codeman.loadWithConst(chai::interpreter::Mulif, 2.0);
    codeman.load(Star, r7);

    // r8 = r6 - r2
    codeman.load(Ldra, r6);
    codeman.load(Subf, r2);
    codeman.load(Star, r8);

    // X1 = r9 = r8 / r7
    codeman.load(Ldra, r8);
    codeman.load(Divf, r7);
    codeman.load(Star, r9);

    // acc = -r2 - r6
    // r11 = acc / r7
    codeman.load(Ldiaf, 0.0);
    codeman.load(Subf, r2);
    codeman.load(Subf, r6);
    codeman.load(Divf, r7);
    codeman.load(Star, r11);
    codeman.load(Ret);
    codeman.update();
}
