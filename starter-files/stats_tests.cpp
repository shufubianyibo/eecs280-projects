/* stats_tests.cpp
 *
 * Unit tests for the simple statistics library
 *
 * EECS 280 Statistics Project
 *
 * Protip #1: Write tests for the functions BEFORE you implement them!  For
 * example, write tests for median() first, and then write median().  It sounds
 * like a pain, but it helps make sure that you are never under the illusion
 * that your code works when it's actually full of bugs.
 *
 * Protip #2: Instead of putting all your tests in main(),  put each test case
 * in a function!
 */


#include "stats.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
using namespace std;

const double epsilon = 0.00001;

static bool almost_equal(double x, double y) {
  return abs(x - y) < epsilon;
}

void test_sum_small_data_set();

// Add prototypes for you test functions here.


int main() {
  test_sum_small_data_set();
  // Call your test functions here

  return 0;
}

void test_sum_small_data_set() {
  cout << "test_sum_small_data_set" << endl;

  vector<double> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);

  assert(count(data) == 3);
  assert(count({1, 2, 3, 4}) == 4);
  assert(count({}) == 0);
  assert(count({-1, -2, -3}) == 3);
  assert(count({1.5, 2.5, 3.5}) == 3);

  assert(sum(data) == 6);
  assert(sum({1, 2, 3, 4}) == 10);
  assert(sum({1, -2, -3}) == -4);
  assert(sum({0, 0, 0}) == 0);
  assert(sum({1.5, 2.5, 3.5}) == 7.5);

  assert(almost_equal(mean(data), 2));
  assert(almost_equal(mean({1, 2, 3, 4}), 2.5));
  assert(almost_equal(mean({1.5, 2.5, 3.5}), 2.5));
  assert(almost_equal(mean({-1, -2, -3}), -2));
  assert(almost_equal(mean({0, 0, 0}), 0));

  assert(median(data) == 2);
  assert(median({1, 2, 3, 4}) == 2.5);
  assert(median({1.5, 2.5, 3.5}) == 2.5);
  assert(median({-1, -2, -3}) == -2);
  assert(median({0, 0, 0}) == 0);

  assert(min(data) == 1);
  assert(min({1, 2, 3, 4}) == 1);
  assert(min({1.5, 2.5, 3.5}) == 1.5);
  assert(min({-1, -2, -3}) == -3);
  assert(min({0, 0, 0}) == 0);

  assert(max(data) == 3);
  assert(max({1, 2, 3, 4}) == 4);
  assert(max({1.5, 2.5, 3.5}) == 3.5);
  assert(max({-1, -2, -3}) == -1);
  assert(max({0, 0, 0}) == 0);

  assert(almost_equal(stdev(data), 1));
  assert(almost_equal(stdev({1, 2, 3, 4}), sqrt(5.0/3.0)));
  assert(almost_equal(stdev({1.5, 2.5, 3.5}), sqrt(1.0)));
  assert(almost_equal(stdev({-1, -2, -3}), 1));
  assert(almost_equal(stdev({0, 0, 0}), 0));
  assert(almost_equal(stdev({1, 1, 1, 1, 1, 1}), 0));

  assert(almost_equal(percentile(data, 0.5), 2));
  assert(almost_equal(percentile({1, 2, 3, 4}, 0.5), 2.5));
  assert(almost_equal(percentile({1.5, 2.5, 3.5}, 0.5), 2.5));
  assert(almost_equal(percentile({-1, -2, -3}, 0.5), -2));
  assert(almost_equal(percentile({0, 0, 0}, 0.5), 0));
  assert(almost_equal(percentile({1, 2, 3, 4, 5}, 1.0), 5));

  assert(filter(data, {1, 0, 1}, 1) == vector<double>({1, 3}));
  assert(filter({1, 2, 3, 4}, {0, 0, 1, 1}, 1) == vector<double>({3, 4}));
  assert(filter({1, 2, 3, 4}, {0, 0, 0, 0}, 1) == vector<double>({}));

  cout << "PASS!" << endl;
}

// Add the test function implementations here.
