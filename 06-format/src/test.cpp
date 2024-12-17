#include <gtest/gtest.h>

#include <iostream>

#include "format.hpp"

TEST(TestBase, TestEmptyArgs) {
  ASSERT_EQ(format("Hello world!!!"), "Hello world!!!");
}

TEST(TestBase, TestOneArgs_int) { ASSERT_EQ(format("{1}", 1), "1"); }

TEST(TestBase, TestOneArgs_str) {
  ASSERT_EQ(format("{1}", "Hello world"), "Hello world");
}

TEST(TestBase, TestMoreArgs) {
  ASSERT_EQ(format("{1}+{2}={3}", 1, 2, 3), "1+2=3");
}

TEST(TestBase, TestMoreArgs_2) {
  ASSERT_EQ(format("{1}+{2}={3}", 1, 2, '3'), "1+2=3");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
