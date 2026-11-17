#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}

// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.hpp as needed.


TEST(test_fill_border_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill_border(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      if (r == 0 || r == height - 1 || c == 0 || c == width - 1) {
        ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
      } else {
        ASSERT_EQUAL(*Matrix_at(&mat, r, c), 0);
      }
    }
  }
}


TEST(test_max_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  Matrix_init(&mat, width, height);
  Matrix_fill(&mat, 1);
  *Matrix_at(&mat, 1, 1) = 42; // Set one element to a higher value

  int max_value = Matrix_max(&mat);
  ASSERT_EQUAL(max_value, 42);
}

TEST(test_max_uniform) {
  Matrix mat;
  Matrix_init(&mat, 3, 3);
  Matrix_fill(&mat, 7);

  ASSERT_EQUAL(Matrix_max(&mat), 7);

}

TEST(test_max_all_negative) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  Matrix_init(&mat, width, height);
  Matrix_fill(&mat, -10);
  *Matrix_at(&mat, 2, 2) = -1; 

  int max_value = Matrix_max(&mat);
  ASSERT_EQUAL(max_value, -1);
}

TEST(test_matrix_print) {
  Matrix mat;
  Matrix_init(&mat, 1, 1);

  *Matrix_at(&mat, 0, 0) = 42;
  ostringstream expected;
  expected << "1 1\n"
           << "42 \n";
  ostringstream actual;
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}

TEST(test_matrix_print_larger) {
  Matrix mat;
  Matrix_init(&mat, 3, 2);

  *Matrix_at(&mat, 0, 0) = 1;
  *Matrix_at(&mat, 0, 1) = 2;
  *Matrix_at(&mat, 0, 2) = 3;
  *Matrix_at(&mat, 1, 0) = 4;
  *Matrix_at(&mat, 1, 1) = 5;
  *Matrix_at(&mat, 1, 2) = 6;

  ostringstream expected;
  expected << "3 2\n"
           << "1 2 3 \n"
           << "4 5 6 \n";
  ostringstream actual;
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}

TEST(test_column_of_min_value_in_row){
  Matrix mat;
  const int width = 4;
  const int height = 3;
  Matrix_init(&mat, width, height);
  *Matrix_at(&mat, 0, 0) = 3;
  *Matrix_at(&mat, 0, 1) = 1;
  *Matrix_at(&mat, 0, 2) = 4;
  *Matrix_at(&mat, 0, 3) = 1;
  
  int min_col = Matrix_column_of_min_value_in_row(&mat, 0, 0, width);
  ASSERT_EQUAL(min_col, 1); 
}

TEST(test_min_value_in_row){
  Matrix mat;
  const int width = 4;
  const int height = 3;
  Matrix_init(&mat, width, height);
  *Matrix_at(&mat, 0, 0) = 3;
  *Matrix_at(&mat, 0, 1) = 1;
  *Matrix_at(&mat, 0, 2) = 4;
  *Matrix_at(&mat, 0, 3) = 1;
  
  int min_value = Matrix_min_value_in_row(&mat, 0, 0, width);
  ASSERT_EQUAL(min_value, 1); 
}

TEST(test_min_value_in_row_negative_values) {
  Matrix mat;
  Matrix_init(&mat, 4, 1);

  *Matrix_at(&mat, 0, 0) = -3;
  *Matrix_at(&mat, 0, 1) = -1;
  *Matrix_at(&mat, 0, 2) = -4;
  *Matrix_at(&mat, 0, 3) = -2;

  int min_value = Matrix_min_value_in_row(&mat, 0, 0, 4);
  ASSERT_EQUAL(min_value, -4);

}

TEST(test_matrix_at) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  Matrix_init(&mat, width, height);
  Matrix_fill(&mat, 1);

  *Matrix_at(&mat, 2, 2) = 42; 

  ASSERT_EQUAL(*Matrix_at(&mat, 2, 2), 42);
}

TEST(test_matrix_at_const) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  Matrix_init(&mat, width, height);
  Matrix_fill(&mat, 1);

  *Matrix_at(&mat, 2, 2) = 42; 

  const Matrix* const_mat = &mat;
  ASSERT_EQUAL(*Matrix_at(const_mat, 2, 2), 42);
}

TEST(test_height_width) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  Matrix_init(&mat, width, height);

  ASSERT_EQUAL(Matrix_width(&mat), width);
  ASSERT_EQUAL(Matrix_height(&mat), height);
}

TEST_MAIN() // Do NOT put a semicolon here
