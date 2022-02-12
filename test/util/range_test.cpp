#include <gtest/gtest.h>
#include <jkds/util/range.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;
using namespace jkds::util;

namespace {
  class RangeTest : public ::testing::Test {
  protected:
    RangeTest() {}
  };

}  // namespace

TEST_F(RangeTest, empty) {
  auto empty_vec(range<size_t>(0));
  EXPECT_EQ(empty_vec.size(), 0);
}

TEST_F(RangeTest, default_start) {
  auto vec(range<size_t>(4));
  std::vector<size_t> range_vec {0, 1, 2, 3};
  EXPECT_EQ(vec, range_vec);
}

TEST_F(RangeTest, custom_start) {
  auto vec(range<size_t>(4, 100));
  std::vector<size_t> range_vec {100, 101, 102, 103};
  EXPECT_EQ(vec, range_vec);
}
