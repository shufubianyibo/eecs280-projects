#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.hpp as needed.
TEST(test_width_height) {
    Image img;
    Image_init(&img, 3, 4);
    ASSERT_EQUAL(Image_width(&img), 3);
    ASSERT_EQUAL(Image_height(&img), 4);
}
TEST(test_get_set_pixel) {
    Image img;
    Image_init(&img, 2, 2);
    Pixel p1 = {100, 150, 200};
    Pixel p2 = {50, 75, 125};
    Image_set_pixel(&img, 0, 0, p1);
    Image_set_pixel(&img, 1, 1, p2);
    ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).r, 100);
    ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).g, 150);
    ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).b, 200);
    ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).r, 50);
    ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).g, 75);
    ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).b, 125);
}
TEST(test_image_fill){
    Image img;
    Image_init(&img, 2, 2);
    Pixel fillColor = {10, 20, 30};
    Image_fill(&img, fillColor);
    for (int row = 0; row < Image_height(&img); ++row) {
        for (int col = 0; col < Image_width(&img); ++col) {
            Pixel p = Image_get_pixel(&img, row, col);
            ASSERT_EQUAL(p.r, 10);
            ASSERT_EQUAL(p.g, 20);
            ASSERT_EQUAL(p.b, 30);
        }
    }
}

TEST(test_load_horses_ppm) {
  Image img;

  // Open the file and initialize the image
  std::ifstream file("horses.ppm");
  ASSERT_TRUE(file.is_open());
  Image_init(&img, file);

  // Verify the dimensions
  ASSERT_EQUAL(Image_width(&img), 479);
  ASSERT_EQUAL(Image_height(&img), 382);

  // Verify a few pixel values
  Pixel p1 = Image_get_pixel(&img, 0, 0); 
  ASSERT_EQUAL(p1.r, 64);
  ASSERT_EQUAL(p1.g, 60);
  ASSERT_EQUAL(p1.b, 59);

  Pixel p2 = Image_get_pixel(&img, 381, 478); 
  ASSERT_EQUAL(p2.r, 141);
  ASSERT_EQUAL(p2.g, 99);
  ASSERT_EQUAL(p2.b, 74);
}


TEST(test_load_crabster_ppm) {
  Image img;

  // Open the file and initialize the image
  std::ifstream file("crabster.ppm");
  ASSERT_TRUE(file.is_open());
  Image_init(&img, file);

  // Verify the dimensions
  ASSERT_EQUAL(Image_width(&img), 74);
  ASSERT_EQUAL(Image_height(&img), 45);

  // Verify a few pixel values
  Pixel p1 = Image_get_pixel(&img, 0, 0); 
  ASSERT_EQUAL(p1.r, 255);
  ASSERT_EQUAL(p1.g, 255);
  ASSERT_EQUAL(p1.b, 255);

  Pixel p2 = Image_get_pixel(&img, 44, 73);
  ASSERT_EQUAL(p2.r, 255);
  ASSERT_EQUAL(p2.g, 255);
  ASSERT_EQUAL(p2.b, 255);

}

TEST(test_image_print_crabster) {
  Image img;

  // Open the file and initialize the image
  std::ifstream file("crabster.ppm");
  ASSERT_TRUE(file.is_open());
  Image_init(&img, file);

  // Capture the output of Image_print
  std::ostringstream output;
  Image_print(&img, output);

  // Reload the original file and manually format it for comparison
  std::ifstream file_again("crabster.ppm");
  ASSERT_TRUE(file_again.is_open());
  std::string format;
  int width, height, max_value;
  file_again >> format >> width >> height >> max_value;

  std::ostringstream expected;
  expected << "P3\n" << width << " " << height << "\n" << max_value << "\n";
  int r, g, b;
  int pixel_count = 0;
  while (file_again >> r >> g >> b) {
    expected << r << " " << g << " " << b;
    ++pixel_count;

    if (pixel_count % width == 0) {
      expected << " \n";
    } else {
      expected << " ";
    }
  }
  // Compare the outputs
  ASSERT_EQUAL(output.str(), expected.str());
}

TEST_MAIN() // Do NOT put a semicolon here
