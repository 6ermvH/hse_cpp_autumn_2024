#include <gtest/gtest.h>

#include "matrix.hh"

class SquareMatrix : public ::testing::Test {
 protected:
  void SetUp() {
    M = Matrix({{1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}});
    D = Matrix({{9, 8, 7},
                {6, 5, 4},
                {3, 2, 1}});
  }

  Matrix M;
  Matrix D;
};

class RectangularMatrix : public ::testing::Test {
 protected:
  void SetUp() {  
    M = Matrix({{1, 2},
                {3, 4},
                {5, 6}});
    D = Matrix({{1, 2, 3}, 
               {4, 5, 6}});
  }
  Matrix M, D;
};

TEST_F(SquareMatrix, test_get_functions) {
  ASSERT_EQ(M.getRows(), 3);
  ASSERT_EQ(M.getColumns(), 3);
}

TEST_F(SquareMatrix, tes_get_elements) {
  ASSERT_EQ(M[0][0], 1);
  ASSERT_EQ(M[0][1], 2);
  ASSERT_EQ(M[0][2], 3);
  ASSERT_EQ(M[1][0], 4);
  ASSERT_EQ(M[1][1], 5);
  ASSERT_EQ(M[1][2], 6);
  ASSERT_EQ(M[2][0], 7);
  ASSERT_EQ(M[2][1], 8);
  ASSERT_EQ(M[2][2], 9);
}

TEST_F(SquareMatrix, tes_set_elements) {
  M[0][0] = 9;
  M[0][1] = 8;
  M[0][2] = 7;
  M[1][0] = 6;
  M[1][1] = 5;
  M[1][2] = 4;
  M[2][0] = 3;
  M[2][1] = 2;
  M[2][2] = 1;
  ASSERT_EQ(M[0][0], 9);
  ASSERT_EQ(M[0][1], 8);
  ASSERT_EQ(M[0][2], 7);
  ASSERT_EQ(M[1][0], 6);
  ASSERT_EQ(M[1][1], 5);
  ASSERT_EQ(M[1][2], 4);
  ASSERT_EQ(M[2][0], 3);
  ASSERT_EQ(M[2][1], 2);
  ASSERT_EQ(M[2][2], 1);
}

TEST_F(SquareMatrix, test_compare) {
  ASSERT_TRUE(M == Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}));
  ASSERT_FALSE(M == Matrix({{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}));
}

TEST_F(SquareMatrix, test_mult) {
  Matrix M_COPY = M;
  M *= 1;
  ASSERT_TRUE(M == M_COPY);
  M *= 4;
  ASSERT_TRUE(M == Matrix({{4, 8, 12}, {16, 20, 24}, {28, 32, 36}}));
}

TEST_F(SquareMatrix, test_sum) {
  ASSERT_TRUE((M + D) == Matrix({{10, 10, 10}, {10, 10, 10}, {10, 10, 10}}));
}

TEST_F(SquareMatrix, test_ostream) {
  std::stringstream ss;
  ss << M;
  std::string line0, line1, line2;
  std::getline(ss, line0);
  std::getline(ss, line1);
  std::getline(ss, line2);

  ASSERT_EQ(line0, std::string("1 2 3 "));
  ASSERT_EQ(line1, std::string("4 5 6 "));
  ASSERT_EQ(line2, std::string("7 8 9 "));
}

TEST_F(RectangularMatrix, test_out_of_bounds_exception) {
  try {
    M[2][2];
  } catch (std::exception& e) {
    ASSERT_STREQ(e.what(), "out of columns");
  }

  try {
    M[3][0];
  } catch (std::exception& e) {
    ASSERT_STREQ(e.what(), "out of rows");
  }
}

TEST_F(RectangularMatrix, test_sum_exception) {
  try {
    M + D;
  } catch (std::exception& e) {
    ASSERT_STREQ(e.what(), "matrix not equal");
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
