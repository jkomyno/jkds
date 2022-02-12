#include <gtest/gtest.h>
#include <jkds/container/sparse_byte_set.h>

#include <cstdint>
#include <limits>
#include <random>
#include <set>

using namespace std;
using namespace jkds::container;

namespace {

  class SparseByteSetTest : public ::testing::Test {
  protected:
    using lims = std::numeric_limits<uint8_t>;
    SparseByteSet s;
  };

}  // namespace

TEST_F(SparseByteSetTest, empty) {
  for (auto c = lims::min(); c < lims::max(); ++c) {
    EXPECT_FALSE(s.contains(c));
  }
}

TEST_F(SparseByteSetTest, each) {
  for (auto c = lims::min(); c < lims::max(); ++c) {
    EXPECT_TRUE(s.add(c));
    EXPECT_TRUE(s.contains(c));
  }

  for (auto c = lims::min(); c < lims::max(); ++c) {
    EXPECT_FALSE(s.add(c));
    EXPECT_TRUE(s.contains(c));
  }
}

TEST_F(SparseByteSetTest, each_random) {
  std::mt19937 rng;
  std::uniform_int_distribution<uint16_t> dist{lims::min(), lims::max()};
  std::set<uint8_t> added;

  while (added.size() <= lims::max()) {
    auto c = uint8_t(dist(rng));
    EXPECT_EQ(added.count(c), s.contains(c));
    EXPECT_EQ(!added.count(c), s.add(c));
    added.insert(c);
    EXPECT_TRUE(added.count(c));
    EXPECT_TRUE(s.contains(c));
  }
}
