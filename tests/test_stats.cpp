#include <gtest/gtest.h>
#include "csv/stats.hpp"

TEST(IsNumeric, AcceptsNumbers) {
    EXPECT_TRUE(is_numeric("3.14"));
    EXPECT_TRUE(is_numeric("-1e3"));
    EXPECT_TRUE(is_numeric("42"));
}

TEST(IsNumeric, RejectsNonNumbers) {
    EXPECT_FALSE(is_numeric(""));
    EXPECT_FALSE(is_numeric("3.14abc"));
    EXPECT_FALSE(is_numeric("N/A"));
}