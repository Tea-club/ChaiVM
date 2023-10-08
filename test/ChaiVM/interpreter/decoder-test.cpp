#include <gtest/gtest.h>

#include "ChaiVM/interpreter/decoder.hpp"

using namespace chai::interpreter;
using chai::interpreter::Instruction;
using namespace chai;

class DecoderCommon : public ::testing::Test {
protected:
    const Decoder decoder{};

    [[nodiscard]] virtual bool equal(Instruction lhs, Instruction rhs) = 0;
    void SetUp() override {}

    void TearDown() override {}
};

class Decoder_I : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.immidiate == rhs.immidiate;
    }

    static Instruction buildInstr(Operation operation, Immidiate imm) {
        return {
            .operation = operation,
            .immidiate = imm,
        };
    }
};

class Decoder_R : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.r1 == rhs.r1;
    }

    static Instruction buildInstr(Operation operation, RegisterId r1) {
        return {
            .operation = operation,
            .r1 = r1,
        };
    }
};

class Decoder_RR : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.r1 == rhs.r1 &&
               lhs.r2 == rhs.r2;
    }

    static Instruction buildInstr(Operation operation, RegisterId r1,
                                  RegisterId r2) {
        return {
            .operation = operation,
            .r1 = r1,
            .r2 = r2,
        };
    }
};

TEST_F(Decoder_I, Addi) {
    const auto instance = buildInstr(Addi, 0xAB2C3D);
    Instruction parsed = decoder.parse(0xAB2C3D07);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, Add) {
    const auto instance = buildInstr(Add, 12);
    Instruction parsed = decoder.parse(0xABCD0C06);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_RR, Mov) {
    const auto instance = buildInstr(Mov, 12, 0x43);
    Instruction parsed = decoder.parse(0xAB430C02);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_RR, NOT_MOV) {
    const auto instance = buildInstr(Mov, 12, 0x43);
    Instruction parsed = decoder.parse(0xAB340C02);
    EXPECT_FALSE(equal(parsed, instance));
}

TEST_F(Decoder_RR, INVALID_OPERATION) {
    Instruction parsed = decoder.parse(0xAB430CFA);
    EXPECT_EQ(parsed.operation, Inv);
}

TEST(Decoder, fetch_decode) {
    Decoder decoder{};
    constexpr int num_words = 5;
    assert(sizeof(chword_t) == 4);
    char *raw = new char[num_words * 4];
    auto *words = (chword_t *)raw;
    // Write words with opcodes to raw.
    Operation operations_seq[num_words] = {Mov, Add, Addi, Sub, Subi};
    for (int i = 0; i < num_words; ++i) {
        words[i] = operations_seq[i];
    }
    for (int i = 0; i < num_words; ++i) {
        assert(raw[i * 4 + 0] != 0);
        assert(raw[i * 4 + 1] == 0);
        assert(raw[i * 4 + 2] == 0);
        assert(raw[i * 4 + 0] != 0);
    }
    chsize_t pc = reinterpret_cast<chsize_t>(raw);
    for (auto &operation : operations_seq) {
        Instruction decoded = decoder.decode(pc);
        EXPECT_EQ(decoded.operation, operation);
        EXPECT_EQ(decoded.r1, 0);
        EXPECT_EQ(decoded.r2, 0);
        EXPECT_EQ(decoded.immidiate, 0);
        pc += 4;
    }
}
