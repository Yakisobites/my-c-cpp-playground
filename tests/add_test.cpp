#include <gtest/gtest.h>

extern "C" {
#include "add.h"
}

TEST(CoreAddTest, AddsPositiveNumbers) { EXPECT_EQ(add(2, 3), 5); }

TEST(CoreAddTest, AddsNegativeNumbers) { EXPECT_EQ(add(-2, -3), -5); }
