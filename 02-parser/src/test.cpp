#include <gtest/gtest.h>

#include <iostream>

#include "parser.hpp"

class ParserTest : public ::testing::Test {
 protected:
  void SetUp() {
    status = 0;
    parser.SetStartCallback([this]() { ++this->status; });
    parser.SetEndCallback([this]() { --this->status; });
    parser.SetDigitTokenCallback(
        [this](uint64_t num) { this->nums.push_back(num); });

    parser.SetStringTokenCallback(
        [this](const std::string& str) { this->strings.push_back(str); });
  }

  TokenParser parser;
  int status;
  std::vector<uint64_t> nums;
  std::vector<std::string> strings;
};

TEST_F(ParserTest, test_one_token) {
  ASSERT_EQ(status, 0);
  parser.Parse("abacabadabacaba");
  ASSERT_EQ(status, 0);
  parser.Parse("123456789");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(nums[0], 123456789ull);
  ASSERT_EQ(strings[0], "abacabadabacaba");
}

TEST_F(ParserTest, test_many_string) {
  ASSERT_EQ(status, 0);
  parser.Parse("123go german632feskov club27 1deeptest1 1deep0test1 0123");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(strings[0], "123go");
  ASSERT_EQ(strings[1], "german632feskov");
  ASSERT_EQ(strings[2], "club27");
  ASSERT_EQ(strings[3], "1deeptest1");
  ASSERT_EQ(strings[4], "1deep0test1");
  ASSERT_EQ(strings[5], "0123");
}

TEST_F(ParserTest, test_many_numbers) {
  ASSERT_EQ(status, 0);
  parser.Parse("1 51512521 5215 125152152151 18446744073709551615 0");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(nums[0], 1);
  ASSERT_EQ(nums[1], 51512521);
  ASSERT_EQ(nums[2], 5215);
  ASSERT_EQ(nums[3], 125152152151);
  ASSERT_EQ(nums[4], 18446744073709551615ull);  // 2**64 - 1
  ASSERT_EQ(nums[5], 0);
}

TEST_F(ParserTest, test_big_numbers) {
  ASSERT_EQ(status, 0);
  parser.Parse(
      "18446744073709551616 999999999999999999999999 "
      "1111111111111111111111111111111111 "
      "000000000000000000000000000000000000000000");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(strings[0], "18446744073709551616");
  ASSERT_EQ(strings[1], "999999999999999999999999");
  ASSERT_EQ(strings[2], "1111111111111111111111111111111111");
  ASSERT_EQ(strings[3], "000000000000000000000000000000000000000000");
}

TEST_F(ParserTest, test_negative_numbers) {
  ASSERT_EQ(status, 0);
  parser.Parse("-41241 -1 -512 -521");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(strings[0], "-41241");
  ASSERT_EQ(strings[1], "-1");
  ASSERT_EQ(strings[2], "-512");
  ASSERT_EQ(strings[3], "-521");
}

TEST_F(ParserTest, test_many_lines_one_token) {
  parser.Parse("aba");
  parser.Parse("141241251205129521502150214210521521");
  parser.Parse("52");
  parser.Parse("52gang");
  parser.Parse("221");
  parser.Parse("5125152");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(strings[0], "aba");
  ASSERT_EQ(strings[1], "141241251205129521502150214210521521");
  ASSERT_EQ(strings[2], "52gang");

  ASSERT_EQ(nums[0], 52);
  ASSERT_EQ(nums[1], 221);
  ASSERT_EQ(nums[2], 5125152);
}

TEST_F(ParserTest, test_many_lines) {
  parser.Parse("aba 21412 fsaf wq 21 41 wq");
  parser.Parse("141241251205129521502150214210521521");
  parser.Parse("gsaf wqfwfqwrq 0512");
  ASSERT_EQ(status, 0);

  ASSERT_EQ(strings[0], "aba");
  ASSERT_EQ(strings[1], "fsaf");
  ASSERT_EQ(strings[2], "wq");
  ASSERT_EQ(strings[3], "wq");
  ASSERT_EQ(strings[4], "141241251205129521502150214210521521");
  ASSERT_EQ(strings[5], "gsaf");
  ASSERT_EQ(strings[6], "wqfwfqwrq");
  ASSERT_EQ(strings[7], "0512");

  ASSERT_EQ(nums[0], 21412);
  ASSERT_EQ(nums[1], 21);
  ASSERT_EQ(nums[2], 41);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
