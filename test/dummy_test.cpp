#include <gtest/gtest.h>

TEST(dummy, dummy_test_success) {
    std::cout << "Hello" << std::endl;
    EXPECT_TRUE(true);
}
