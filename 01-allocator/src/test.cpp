#include <gtest/gtest.h>

#include <iostream>

#include "allocator.hpp"

#define TEST_1
#define TEST_2
#define TEST_3
#define TEST_4

#ifdef TEST_1
TEST(TestBase, Create_Delete) {
  {
    Allocator alloc;
    ASSERT_TRUE(alloc.alloc(0) == nullptr);
  }
  {
    Allocator alloc;
    alloc.makeAllocator(1);
    ASSERT_TRUE(alloc.alloc(0) != nullptr);
  }
  {
    Allocator alloc;
    alloc.makeAllocator(10);
    ASSERT_TRUE(alloc.alloc(0) != nullptr);
  }
  {
    Allocator alloc;
    alloc.makeAllocator(1e5);
    ASSERT_TRUE(alloc.alloc(0) != nullptr);
  }
  {
    Allocator alloc;
    alloc.makeAllocator(1e6);
    ASSERT_TRUE(alloc.alloc(0) != nullptr);
  }
}
#endif  // TEST_1

#ifdef TEST_2
TEST(TestMakeAlloc, Realloc) {
  Allocator alloc;
  alloc.makeAllocator(100);
  ASSERT_TRUE(alloc.alloc(100) != nullptr);
  ASSERT_TRUE(alloc.alloc(1) == nullptr);
  alloc.makeAllocator(150);
  ASSERT_TRUE(alloc.alloc(150) != nullptr);
  ASSERT_TRUE(alloc.alloc(1) == nullptr);
}
#endif  // TEST_2

#ifdef TEST_3
TEST(TestAlloc, Allocate) {
  Allocator alloc;
  alloc.makeAllocator(100);
  for (size_t i = 0; i < 100; ++i) {
    ASSERT_TRUE(alloc.alloc(1) != nullptr);
  }
  ASSERT_TRUE(alloc.alloc(1) == nullptr);
}
#endif  // TEST_3

#ifdef TEST_4
TEST(TestReset, Reset) {
  Allocator alloc;
  alloc.makeAllocator(100);
  for (size_t i = 0; i < 100; ++i) {
    ASSERT_TRUE(alloc.alloc(1) != nullptr);
  }
  ASSERT_TRUE(alloc.alloc(1) == nullptr);

  alloc.reset();
  for (size_t i = 0; i < 100; ++i) {
    ASSERT_TRUE(alloc.alloc(1) != nullptr);
  }
  ASSERT_TRUE(alloc.alloc(1) == nullptr);
}
#endif  // TEST_4

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
