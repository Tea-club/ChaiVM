#include <gtest/gtest.h>

#include "ChaiVM/interpreter/decoder.hpp"

using namespace chai::interpreter;
using chai::interpreter::Instruction;

class DecoderCommon : public ::testing::Test {
protected:
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

TEST_F(Decoder_I, imm) {
    const auto instance = buildInstr(Addi, -3); // FFFFFFFD
    Instruction parsed = decoder::parse(0x000000FFFFFFFD08);
    char * arr = (char *)malloc(10);
    EXPECT_TRUE(equal(parsed, instance));
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
}

TEST_F(Decoder_I, Addi) {
    const auto instance = buildInstr(Addi, 0xFAAB2C3D);
    Instruction parsed = decoder::parse(0xD00000FAAB2C3D08);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, Add) {
    const auto instance = buildInstr(Add, 12);
    Instruction parsed = decoder::parse(0xD000000000000C07);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_RR, Mov) {
    const auto instance = buildInstr(Mov, 12, 0x43);
    Instruction parsed = decoder::parse(0xD000000000430C03);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_RR, NOT_MOV) {
    const auto instance = buildInstr(Mov, 12, 0x43);
    Instruction parsed = decoder::parse(0xD000000000340C03);
    EXPECT_FALSE(equal(parsed, instance));
}

TEST_F(Decoder_RR, INVALID_OPERATION) {
    Instruction parsed = decoder::parse(0xADBBEEFAAB430CFA);
    EXPECT_EQ(parsed.operation, Inv);
}
