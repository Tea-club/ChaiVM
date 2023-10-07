#include "ChaiVM/utils/bit-magic.hpp"
#include <gtest/gtest.h>

TEST(ExtractBits, BitMagic) {
    unsigned int v = 0b00111100111;
    auto r = chai::utils::ExtractBits<unsigned int, 4, 5>(v);
    EXPECT_EQ(r, 0b1111);
}
