#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "Image.hpp"
#include "Matrix.hpp"
#include "processing.hpp" 
using namespace std;

int main(int argc, char *argv[]) {
  cout << "Hello World!\n";
  string input_filename = argv[1];
  string output_filename = argv[2];
  int new_width = atoi(argv[3]);
  int new_height = atoi(argv[4]);
  ifstream is(input_filename);
  assert(is.is_open());
  Image img;
  Image_init(&img, is);
  is.close();
  seam_carve(&img, new_width, new_height);
  ofstream os(output_filename);
  assert(os.is_open()); 
  Image_print(&img, os); 
  os.close(); 

}
