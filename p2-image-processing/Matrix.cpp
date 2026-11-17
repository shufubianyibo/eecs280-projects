#include <cassert>
#include "Matrix.hpp"

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  assert(width > 0 && height > 0); 
  mat->width = width;
  mat->height = height;
  mat->data = std::vector<int>(width * height, 0);
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  assert(mat != nullptr);
  os << mat->width << " " << mat->height << "\n";
  for (int i = 0; i < mat->height; i++) {
    for (int j = 0; j < mat->width; j++) {
      os << mat->data[i * mat->width + j] << " ";
    }
    os << "\n";
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  assert(mat != nullptr); 
  return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  assert(mat != nullptr); 
  return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  assert(0 <= row && row < mat->height);
  assert(0 <= column && column < mat->width);
  return &mat->data[row * mat->width + column];
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  assert(0 <= row && row < mat->height);
  assert(0 <= column && column < mat->width);
  return &mat->data[row * mat->width + column];
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  assert(mat != nullptr);
  for (int i = 0; i < mat->height; i++) {
    for (int j = 0; j < mat->width; j++) {
      mat->data[i * mat->width + j] = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  assert(mat != nullptr);
  for (int i = 0; i < mat->height; i++) {
    for (int j = 0; j < mat->width; j++) {
      if (j == 0 || j == mat->width - 1 || i == 0 || i == mat->height - 1) {
        mat->data[i * mat->width + j] = value;
      }
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  assert(mat != nullptr);
  int max = mat->data[0];
  for (size_t i = 0; i < mat->height; i++){
    for (int j = 0; j < mat->width; j++) {
      if (mat->data[i*mat->width+j] > max) {
        max = mat->data[i*mat->width+j];
      }
    }
  }
  return max;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  assert(mat != nullptr); 
  assert(0 <= row && row < mat->height);
  assert(0 <= column_start && column_end <= mat->width);
  assert(column_start < column_end);
  int min_col = column_start;
  int min_value = *Matrix_at(mat, row, min_col);
  for (size_t i = column_start; i < column_end; i++){
    int current_value = *Matrix_at(mat, row, i);
    if (current_value < min_value){
      min_value = current_value;
      min_col = i;
    }
    
  }
  return min_col;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  assert(mat != nullptr); 
  assert(0 <= row && row < mat->height);
  assert(0 <= column_start && column_end <= mat->width);
  assert(column_start < column_end);
  int min = *Matrix_at(mat, row, column_start);
  for (size_t i = column_start; i < column_end; i++){
    if (*Matrix_at(mat, row, i) < min) {
      min = *Matrix_at(mat, row, i);
    }
    
  }
  return min;
}
