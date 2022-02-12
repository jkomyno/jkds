#include <gtest/gtest.h>
#include <jkds/functional/zip.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;
using namespace jkds::functional;

namespace {
  class ZipTest : public ::testing::Test {
  protected:
    ZipTest() {}
  };

}  // namespace

TEST_F(ZipTest, empty) {
  std::vector<std::tuple<std::size_t, uint8_t>> pairs;
  const std::vector<std::size_t> first;
  const std::vector<std::uint8_t> second;

  for (auto&& [a, b] : zip(first, second)) {
    pairs.push_back(make_tuple(a, b));
  }

  EXPECT_EQ(pairs.size(), 0);
}

TEST_F(ZipTest, three) {
  const std::vector<std::size_t> first{1, 2, 3, 4, 5};
  const std::vector<std::uint8_t> second{2, 4, 6, 8, 10, 12};
  std::vector<std::uint16_t> third(4);

  for (auto&& [a, b, c] : zip(first, second, third)) {
    c = a + b;
  }

  EXPECT_EQ(third, (std::vector<std::uint16_t>{3, 6, 9, 12}));
}
