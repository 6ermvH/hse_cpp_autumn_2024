#include <gtest/gtest.h>

#include "bigint.hpp"

//run this test with valgrind or check-memory flag
TEST(TestBase, Construct_Destruct) {
  {
    BigInt num("12345678901234");
    ASSERT_EQ("12345678901234", num.to_string());
  }
  {
    BigInt num("-42152151");
    ASSERT_EQ("-42152151", num.to_string());
  }
  {
    BigInt num(42141);
    ASSERT_EQ("42141", num.to_string());
  }
  {
    BigInt num(-612);
    ASSERT_EQ("-612", num.to_string());
  }
  {
    BigInt num(0);
    ASSERT_EQ("0", num.to_string());
  }
}

TEST(TestOperator, Compare) {
  ASSERT_TRUE(BigInt("100") > BigInt("99"));
  ASSERT_TRUE(BigInt("100") >= BigInt("100"));
  ASSERT_FALSE(BigInt("100") > BigInt("100"));

  ASSERT_TRUE(BigInt("100") == BigInt(100));
  ASSERT_TRUE(BigInt("-100") != BigInt(100));

  ASSERT_FALSE(BigInt("100") == BigInt("101"));
  ASSERT_FALSE(BigInt("-100") != BigInt(-100));

  ASSERT_TRUE(BigInt("100") == 100);
  ASSERT_TRUE(BigInt("-100") == -100);
}

TEST(TestOperator, Unary) {
  ASSERT_TRUE(-BigInt("100") == BigInt(-100));
}

TEST(TestOperator, Plus) {
  ASSERT_EQ(BigInt("100") + BigInt("100"), BigInt("200"));
  ASSERT_EQ(BigInt("100") + BigInt("-100"), BigInt("0"));
  ASSERT_EQ(BigInt("-100") + BigInt("100"), BigInt("0"));
  ASSERT_EQ(BigInt("-100") + BigInt("-100"), BigInt("-200"));
}

TEST(TestOperator, Minus) {
  ASSERT_EQ(BigInt("100") - BigInt("100"), BigInt("0"));
  ASSERT_EQ(BigInt("100") - BigInt("-100"), BigInt("200"));
  ASSERT_EQ(BigInt("-100") - BigInt("100"), BigInt("-200"));
  ASSERT_EQ(BigInt("-100") - BigInt("-100"), BigInt("0"));
}

TEST(TestOperator, Mult) {
  ASSERT_EQ(BigInt("100") * BigInt("100"), BigInt("10000"));
  ASSERT_EQ(BigInt("100") * BigInt("0"), BigInt("0"));
  ASSERT_EQ(BigInt("0") * BigInt("100"), BigInt("0"));
  ASSERT_EQ(BigInt("0") * BigInt("0"), BigInt("0"));
  ASSERT_EQ(BigInt("100") * BigInt("-100"), BigInt("-10000"));
}

TEST(TestOperator, Out) {
  {
    std::stringstream ss;
    ss << BigInt(100);
    ASSERT_EQ(ss.str(), "100");
  }
  {
    std::stringstream ss;
    ss << BigInt(-100);
    ASSERT_EQ(ss.str(), "-100");
  }
  {
    std::stringstream ss;
    ss << BigInt(0);
    ASSERT_EQ(ss.str(), "0");
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
