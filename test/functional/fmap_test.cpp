#include <gtest/gtest.h>
#include <jkds/functional/fmap.h>

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace jkds::functional;

namespace {

  struct Pod {
    uint8_t value;

    Pod(uint8_t value) : value(value) {
    }
  };

  class FmapTest : public ::testing::Test {
  protected:
    FmapTest() {
    }
  };

}  // namespace

TEST_F(FmapTest, empty) {
  std::vector<uint8_t> in;
  auto out = fmap(
      [](uint8_t in_value) {
        return in_value;
      },
      in);

  EXPECT_EQ(out.size(), 0);
}

TEST_F(FmapTest, copy_non_empty) {
  const vector<int> numbers{1, 2, 3, 4, 5};
  const vector<string> strings{"1", "2", "3", "4", "5"};

  const auto out_strings = fmap([](int n) { return to_string(n); }, numbers);
  const auto out_numbers = fmap([](const string& s) { return atoi(s.c_str()); }, strings);

  EXPECT_EQ(numbers.size(), 5);
  EXPECT_EQ(strings.size(), 5);

  EXPECT_EQ(out_numbers, numbers);
  EXPECT_EQ(out_strings, strings);
}

TEST_F(FmapTest, range_non_empty) {
  const vector<int> numbers{1, 2, 3, 4, 5};
  const vector<string> strings{"1", "2", "3"};

  const auto out_strings = fmap([](int n) { return to_string(n); }, numbers.cbegin(), numbers.cbegin() + 3);

  EXPECT_EQ(numbers.size(), 5);
  EXPECT_EQ(out_strings, strings);
}
