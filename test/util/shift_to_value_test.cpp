#include <gtest/gtest.h>
#include <jkds/util/shift_to_value.h>

#include <cstdint>
#include <vector>

using namespace std;

namespace {
  class ShiftToValueTest : public ::testing::Test {
  };
}  // namespace


TEST_F(ShiftToValueTest, distinct) {
  std::vector<uint8_t> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(vec.size(), 10);

  jkds::util::shift_to_value(vec, 5);

  EXPECT_EQ(vec.size(), 10);
  EXPECT_EQ(vec, std::vector<uint8_t>({5, 6, 7, 8, 9, 0, 1, 2, 3, 4}));
}

TEST_F(ShiftToValueTest, repeated_selects_first) {
  std::vector<uint8_t> vec{0, 10, 10, 10, 20};
  EXPECT_EQ(vec.size(), 5);

  jkds::util::shift_to_value(vec, 10);

  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec, std::vector<uint8_t>({10, 10, 10, 20, 0}));
}

TEST_F(ShiftToValueTest, not_found) {
  std::vector<uint8_t> vec{0, 10, 10, 10, 20};
  EXPECT_EQ(vec.size(), 5);

  jkds::util::shift_to_value(vec, 1);

  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec, std::vector<uint8_t>({0, 10, 10, 10, 20}));
}
