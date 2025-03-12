#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

class GarbageCollectorTest : public ::testing::Test {
protected:
    static constexpr chai::interpreter::RegisterId R0 = 0;
    static constexpr chai::interpreter::RegisterId R1 = 1;
    static constexpr chai::interpreter::RegisterId R2 = 2;
    static constexpr chai::interpreter::RegisterId R3 = 3;
    static constexpr chai::interpreter::RegisterId R4 = 4;
    static constexpr chai::interpreter::RegisterId R5 = 5;
    static constexpr chai::interpreter::RegisterId R6 = 6;
    static constexpr chai::interpreter::RegisterId R7 = 7;
    static constexpr chai::interpreter::RegisterId R8 = 8;
    static constexpr chai::interpreter::RegisterId R9 = 9;
    static constexpr chai::interpreter::RegisterId R10 = 10;
    static constexpr chai::interpreter::RegisterId R11 = 11;
    std::filesystem::path path_;

    /**
     * These methods loads the operation in template parameter with the
     * corresponding parameters.
     * @param op Operation.
     * @return Number of added instruction among all instructions.
     */
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::R,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1) {
        return loadRR(op, reg1, 0);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::RR,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1,
         chai::interpreter::RegisterId reg2) {
        return loadRR(op, reg1, reg2);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::RI,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1, chai::interpreter::Immidiate imm) {
        return loadRI(op, reg1, imm);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::I,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::Immidiate imm) {
        return loadI(op, imm);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::N,
                            chai::interpreter::Immidiate>::type
    load() {
        return loadN(op);
    }

    // @todo #54:60min do something to add this functionality to load Immediate
    // (todo duplication)
    void loadWithConst(chai::interpreter::Operation op, int64_t data);

    void loadWithConst(chai::interpreter::Operation op, double data);

    void update();

    void SetUp() override;

    void TearDown() override;

private:
    chai::interpreter::Immidiate loadRR(chai::interpreter::Operation op,
                                        chai::interpreter::RegisterId reg1,
                                        chai::interpreter::RegisterId reg2 = 0);
    chai::interpreter::Immidiate loadRI(chai::interpreter::Operation op,
                                        chai::interpreter::RegisterId reg1,
                                        chai::interpreter::Immidiate imm);
    chai::interpreter::Immidiate loadI(chai::interpreter::Operation op,
                                       chai::interpreter::Immidiate imm);
    chai::interpreter::Immidiate loadN(chai::interpreter::Operation op);

protected:
    size_t numOfRegs = 50;
    size_t numOfFrames = 256;
    size_t objBufSize = 300;
    chai::utils::fileformat::ChaiFile chaiFile_;
    chai::interpreter::CodeManager codeManager_;
    chai::memory::LinearBuffer frameBuffer_ = chai::memory::LinearBuffer(
        numOfFrames * (numOfRegs * sizeof(chai::chsize_t) +
                       sizeof(chai::interpreter::Frame)));
    chai::memory::TracedByteAllocator objectsAlocator{objBufSize};
    chai::memory::LinearBuffer primitivesBuffer =
        chai::memory::LinearBuffer(1024 * 256);
    chai::interpreter::Executor exec_{&codeManager_, frameBuffer_,
                                      primitivesBuffer, objectsAlocator};
};

using chai::interpreter::Immidiate;

Immidiate GarbageCollectorTest::loadRR(chai::interpreter::Operation op,
                                       chai::interpreter::RegisterId reg1,
                                       chai::interpreter::RegisterId reg2) {
    return chaiFile_.addInstr(chai::utils::instr2RawRR(op, reg1, reg2));
}

Immidiate GarbageCollectorTest::loadRI(chai::interpreter::Operation op,
                                       chai::interpreter::RegisterId reg1,
                                       chai::interpreter::Immidiate imm) {
    return chaiFile_.addInstr(chai::utils::instr2RawRI(op, reg1, imm));
}

Immidiate GarbageCollectorTest::loadI(chai::interpreter::Operation op,
                                      chai::interpreter::Immidiate imm) {
    return chaiFile_.addInstr(chai::utils::instr2RawI(op, imm));
}

Immidiate GarbageCollectorTest::loadN(chai::interpreter::Operation op) {
    return chaiFile_.addInstr(chai::utils::instr2RawN(op));
}

void GarbageCollectorTest::loadWithConst(chai::interpreter::Operation op,
                                         int64_t data) {
    chaiFile_.addWithConst(op, data);
}

void GarbageCollectorTest::loadWithConst(chai::interpreter::Operation op,
                                         double data) {
    chaiFile_.addWithConst(op, data);
}

void GarbageCollectorTest::update() {
    chaiFile_.toFile(path_);
    codeManager_.load(path_);
}

void GarbageCollectorTest::SetUp() {
    path_ = std::string{"test_"}.append(std::string{
        testing::UnitTest::GetInstance()->current_test_info()->name()});
    std::remove(path_.c_str());
}

void GarbageCollectorTest::TearDown() { std::remove(path_.c_str()); }

using namespace chai;
using namespace interpreter;
using namespace utils::fileformat;

TEST_F(GarbageCollectorTest, CollectRoots) {
    auto bar_klass = chaiFile_.registerKlass("Bar");
    chaiFile_.addField(bar_klass, "bar.num1", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num2", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num3", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num4", 0U, FieldTag::I64);
    constexpr int64_t threshold = 4000;
    Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    Immidiate threshold_imm =
        chaiFile_.addConst(std::make_unique<ConstI64>(threshold));

    load<Ldia>(threshold_imm);
    load<Star>(R10);
    load<AllocRef>(bar_klass);
    load<Ldra>(R2);
    load<Addi>(one_imm);
    load<Star>(R2);
    load<If_icmplt>(R10, static_cast<Immidiate>(-4));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}

TEST_F(GarbageCollectorTest, SingleArray) {
    Immidiate array_size = chaiFile_.addConst(std::make_unique<ConstI64>(30));

    load<Ldia>(array_size);
    load<NewRefArray>();
    load<StarRef>(R11);
    load<Ret>();
    update();
    EXPECT_NO_THROW(exec_.run());
}

TEST_F(GarbageCollectorTest, ArrayAllocation) {
    constexpr int64_t threshold = 4000;
    Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    Immidiate array_size = chaiFile_.addConst(std::make_unique<ConstI64>(30));
    Immidiate threshold_imm =
        chaiFile_.addConst(std::make_unique<ConstI64>(threshold));

    load<Ldia>(threshold_imm);
    load<Star>(R10);
    load<Ldia>(array_size);
    load<NewRefArray>();
    load<Ldra>(R2);
    load<Addi>(one_imm);
    load<Star>(R2);
    load<If_icmplt>(R10, static_cast<Immidiate>(-5));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}

TEST_F(GarbageCollectorTest, ArrayWithObjectsAllocation) {
    auto bar_klass = chaiFile_.registerKlass("Bar");
    chaiFile_.addField(bar_klass, "bar.num1", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num2", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num3", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num4", 0U, FieldTag::I64);
    constexpr int64_t threshold = 1000;
    Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    Immidiate array_size = chaiFile_.addConst(std::make_unique<ConstI64>(20));
    Immidiate threshold_imm =
        chaiFile_.addConst(std::make_unique<ConstI64>(threshold));

    load<Ldia>(array_size);
    load<NewRefArray>();
    load<StarRef>(R11);
    load<Ldia>(threshold_imm);
    load<Star>(R10);
    load<AllocRef>(bar_klass);
    load<Ldra>(R2);
    load<Addi>(one_imm);
    load<Star>(R2);
    load<If_icmplt>(R10, static_cast<Immidiate>(-4));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}
