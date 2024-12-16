#include "serialize.hpp"

#include <gtest/gtest.h>

#include <iostream>

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

TEST(TestBase, TestSerialize) {
  Data a = {1, false, 2};
  std::stringstream ss;
  Serializer serialize(ss);
  serialize.save(a);
  ASSERT_EQ(ss.str(), "1 false 2 ");
}

TEST(TestBase, TestSerialize_2) {
  Data a = {std::numeric_limits<uint64_t>::max(), true, std::numeric_limits<uint64_t>::min()};
  std::stringstream ss;
  Serializer serialize(ss);
  serialize.save(a);
  ASSERT_EQ(ss.str(), "18446744073709551615 true 0 ");
}

TEST(TestBase, TestDeSerialize) {
  std::stringstream ss;
  ss << 1 << " true " << 2;
  Data a;
  Deserializer b(ss);
  b.load(a);
  ASSERT_EQ(a.a, 1);
  ASSERT_EQ(a.b, true);
  ASSERT_EQ(a.c, 2);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
