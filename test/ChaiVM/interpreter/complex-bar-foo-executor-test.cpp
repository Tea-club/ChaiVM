#include "executor-test-fixture.hpp"

using namespace chai;
using namespace interpreter;
using namespace utils;
using namespace fileformat;

/*
class Bar {
    public a: number = 0
}

class Foo {
    public x: number = 0
    public y: Bar | null = null
}

function dump(foo: Foo[]): void {
    for (let i : number = 0; i < foo.length; i++) {
        let f = new Foo()
        f = foo[i]
        if (f == null) {
            console.log("Foo:null")
        } else {
            let b = f.y
            if (b == null) {
                console.log("Foo.Bar:null")
            } else {
                console.log(b.a)
            }
        }
    }
}

function foo(N : number, M : number) : void {
    let arr : Foo[] = new Array<Foo>(M)
    let outer : Foo | null = null

    for (let i : number = 1; i <= N; i++) {
        let o1 = new Foo()
        o1.x = i
        if (i % 3 == 0) {
            arr[i % M - 1] = o1
        }

        let o2 = new Bar()
        o2.a = i
        if (i % 5 == 0) {
            o1.y = o2
        }

        outer = o1

        dump(foo)
    }
}

function main() : void {
    let N : number = 4_000_000
    let M : number = 1_000
    foo(N, M)
}
 */
class SpecificBarFooExecutorTest : public ExecutorTest {

protected:
    static constexpr chai::chsize_t N = 400;
    static constexpr chai::chsize_t M = 100;

    Immidiate bar_klass_ = 0;
    Immidiate foo_klass_ = 0;
    Immidiate dump_ = 0;
    Immidiate foo_ = 0;

    /**
     * Init Foo and Bar klasses.
     */
    void initKlasses() {
        assert(bar_klass_ == 0);
        assert(foo_klass_ == 0);
        assert(dump_ == 0);
        bar_klass_ = chaiFile_.registerKlass("Bar");
        chaiFile_.addField(bar_klass_, "bar.a", 0U, FieldTag::I64);
        foo_klass_ = chaiFile_.registerKlass("Foo");
        chaiFile_.addField(foo_klass_, "Foo.x", 0U, FieldTag::I64);
        chaiFile_.addField(foo_klass_, "Foo.y", 1, bar_klass_);
    }

    /**
     * Init dump function.
     *
     * arr = getAcc()
     * R2 = getAcc()     // arr
     * R1 = arr.len
     * ITER = 0
     * start_         // start loop
     * print ITER
     * getAcc() = new Foo()
     * getAcc() = R2 [i]
     * // if-else block
     * ifnull getAcc (+9)
     * acc = getAcc.getField(8)
     * ifnull getAcc (+4)
     * acc = getAcc.getField(0)
     * ic_print
     * Ret
     * Ldia ("Foo.Bar:null")
     * StringPrint
     * Ret
     * Ldia ("Foo:null")
     * StringPrint
     * Ret
     * // if-else block
     * getAcc() = ITER
     * getAcc()++
     * if_icmplt R1 -100 (go to start)
     * Ret
     *
     * @return Immidiate of the function.
     */
    void initDump() {
        static constexpr RegisterId ITER = R11;
        Immidiate zero_imm = chaiFile_.addConst(std::make_unique<ConstI64>(0));
        Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
        Immidiate str1_imm =
            chaiFile_.addConst(std::make_unique<ConstRawStr>("Foo.Bar:null"));
        Immidiate str2_imm =
            chaiFile_.addConst(std::make_unique<ConstRawStr>("Foo:null"));
        dump_ = chaiFile_.addFunction(
            UINT16_MAX, "dump", "([Foo)",
            std::vector<bytecode_t>{
                instr2Raw<StarRef>(R2),
                instr2Raw<GetField>(0), // arr.len
                instr2Raw<Star>(R1), instr2Raw<Ldia>(zero_imm),
                instr2Raw<Star>(ITER),
                // start cycle
                instr2Raw<Star>(ITER), instr2Raw<IcPrint>(),
                instr2Raw<AllocRef>(foo_klass_), instr2Raw<LdraRef>(R2),
                instr2Raw<GetRefFromArr>(ITER),
                // if-else block
                instr2Raw<If_null>(+9), instr2Raw<GetField>(8),
                instr2Raw<If_null>(+4), instr2Raw<GetField>(0),
                instr2Raw<IcPrint>(), instr2Raw<Goto>(+3),
                instr2Raw<Ldia>(str1_imm), instr2Raw<StringPrint>(),
                instr2Raw<Goto>(+3), instr2Raw<Ldia>(str2_imm),
                instr2Raw<StringPrint>(), instr2Raw<Goto>(+1),
                // if-else block
                instr2Raw<Ldra>(ITER), instr2Raw<Addi>(one_imm),
                instr2Raw<If_icmplt>(R1, -19), instr2Raw<Ret>()},
            0, 12);
    }

    /**
     * Init foo() function.
     *
     * input: getAcc() = N, R11 = M
     * R10 = N                         // R10 = N, R11 = M
     * Ldra r11
     * NewRefArray Foo
     * StarRef R9                       // R9 = arr
     * AllocRef Foo
     * OUTER = new Foo                  // OUTER == R8
     * ITER = 1                         // ITER = R7
     * start_                           // start loop
     * getAcc = O1 = new Foo               // O1 = R6
     * setField(ITER, 0); Star O1
     * getAcc = ITER; MODI three
     * if_icmpne R0 (offset second_if_)
     * R5 = i % M - 1
     * R9[R5] = O1
     * second_if_:
     * getAcc = O2 = new Bar               // O2 = R4
     * setfield (ITER, 0)
     * Star O2
     * ldra ITER
     * MODI five
     * if_cmpne R0 +3
     * getAcc = O1; SetField(O2, 8)
     * StarRef O1
     * LdraRef R9
     * Call dump
     * getAcc = ITER; acc++; Star ITER
     * if_icmple R1 -100 (go to start)
     * Ret
     */
    void initFoo() {
        Immidiate three_imm = chaiFile_.addConst(std::make_unique<ConstI64>(3));
        Immidiate five_imm = chaiFile_.addConst(std::make_unique<ConstI64>(5));
        Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
        Immidiate null_imm = chaiFile_.addConst(std::make_unique<ConstI64>(0));
        constexpr RegisterId OUTER = R8;
        constexpr RegisterId ITER = R7;
        constexpr RegisterId O1 = R6;
        constexpr RegisterId O2 = R4;
        foo_ = chaiFile_.addFunction(
            UINT16_MAX, "foo", "(II)",
            std::vector<bytecode_t>{
                instr2Raw<Star>(R10), instr2Raw<Ldra>(R11),
                instr2Raw<NewRefArray>(), instr2Raw<StarRef>(R9),
                instr2Raw<Ldia>(null_imm), instr2Raw<Star>(OUTER),
                instr2Raw<Ldia>(one_imm), instr2Raw<Star>(ITER),
                // start cycle
                instr2Raw<Ldra>(ITER), instr2Raw<IcPrint>(),
                instr2Raw<AllocRef>(foo_klass_), instr2Raw<SetField>(ITER, 0),
                instr2Raw<StarRef>(O1), instr2Raw<Ldra>(ITER),
                instr2Raw<Modi>(three_imm), instr2Raw<If_icmpne>(R0, +7),
                instr2Raw<Ldra>(ITER), instr2Raw<Mod>(R11),
                instr2Raw<Subi>(one_imm), instr2Raw<Star>(R5),
                instr2Raw<LdraRef>(R9), instr2Raw<SetRefInArr>(R5, O1),
                // second_if_
                instr2Raw<AllocRef>(bar_klass_), instr2Raw<SetField>(ITER, 0),
                instr2Raw<StarRef>(O2), instr2Raw<Ldra>(ITER),
                instr2Raw<Modi>(five_imm), instr2Raw<If_icmpne>(R0, +3),
                instr2Raw<LdraRef>(O1), instr2Raw<SetField>(O2, 8),
                instr2Raw<LdraRef>(O1), instr2Raw<StarRef>(OUTER),
                instr2Raw<LdraRef>(R9), instr2Raw<Call>(dump_),
                instr2Raw<Ldra>(ITER), instr2Raw<Addi>(one_imm),
                instr2Raw<Star>(ITER), instr2Raw<If_icmple>(R10, -28),
                instr2Raw<Ret>()},
            1, 12);
    }
};

TEST_F(SpecificBarFooExecutorTest, DumpCall_1) {
    initKlasses();
    initDump();
    int len = 100;
    Immidiate imm_len = chaiFile_.addConst(std::make_unique<ConstI64>(len));
    load<Ldia>(imm_len);
    load<Star>(R1);
    load<NewRefArray>();
    load<Call>(dump_);
    load<Ret>();
    update();
    exec_.run();
}

/*
 * R1 = arr = new Foo[10]
 * arr[3] = new Foo()
 * R6 = new Foo()
 * R5 = new Bar()
 * R4 = 112
 * R5.a = 112
 * R6.y = R5
 * R7 = 5
 * R1[R7] = R6
 */
TEST_F(SpecificBarFooExecutorTest, DumpCall_2) {
    initKlasses();
    initDump();
    int len = 10;
    Immidiate len_imm = chaiFile_.addConst(std::make_unique<ConstI64>(len));
    int index1 = 3;
    Immidiate index1_imm =
        chaiFile_.addConst(std::make_unique<ConstI64>(index1));
    load<Ldia>(index1_imm);
    load<Star>(R3); // R3 = 3
    load<AllocRef>(foo_klass_);
    load<StarRef>(R2); // R2 = new Foo
    load<Ldia>(len_imm);
    load<NewRefArray>();
    load<SetRefInArr>(R3, R2);
    load<StarRef>(R1);

    int val = 112;
    Immidiate val_imm = chaiFile_.addConst(std::make_unique<ConstI64>(val));
    load<Ldia>(val_imm);
    load<Star>(R4);

    load<AllocRef>(bar_klass_);
    load<SetField>(R4, 0);
    load<StarRef>(R5); // R5 = Bar, R5.a = 112

    load<AllocRef>(foo_klass_);
    load<SetField>(R5, 8);
    load<StarRef>(R6);

    int index2 = 6;
    Immidiate index2_imm =
        chaiFile_.addConst(std::make_unique<ConstI64>(index2));
    load<Ldia>(index2_imm);
    load<Star>(R7);

    load<LdraRef>(R1);
    load<SetRefInArr>(R7, R6);

    load<Call>(dump_);
    load<Ret>();
    update();
    exec_.run();
}

TEST_F(SpecificBarFooExecutorTest, FooCall_1) {
    initKlasses();
    initDump();
    initFoo();

    Immidiate n_imm = chaiFile_.addConst(std::make_unique<ConstI64>(100));
    Immidiate m_imm = chaiFile_.addConst(std::make_unique<ConstI64>(10));
    load<Ldia>(m_imm);
    load<Star>(99);
    load<Ldia>(n_imm);
    load<Call>(foo_);
    load<Ret>();
    update();
    exec_.run();
}
