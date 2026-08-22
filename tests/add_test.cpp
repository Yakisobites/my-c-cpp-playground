#include <gtest/gtest.h>

extern "C" {
#include "add.h"
}

TEST(CoreAddTest, AddsPositiveNumbers) { EXPECT_EQ(5, add(2, 3)); }

TEST(CoreAddTest, AddsNegativeNumbers) { EXPECT_EQ(-5, add(-2, -3)); }
