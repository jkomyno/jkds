#include <gtest/gtest.h>
#include <jkds/util/resize.h>

#include <vector>

using namespace std;

namespace {
  class ResizeTest : public ::testing::Test {
  };

  struct Pod {
    char value;

    Pod() = delete;
    Pod(char value) : value(value) {
    }

    Pod(const Pod& other) = default;
    auto operator<=>(const Pod& rhs) const = default;
  };
}  // namespace

/*
TEST_F(ResizeTest, does_not_compile) {
  std::vector<Pod> vec{Pod('a'), Pod('b'), Pod('c'), Pod('d')};
  vec.resize(2);
}
*/

TEST_F(ResizeTest, full) {
  std::vector<Pod> vec{Pod('a'), Pod('b'), Pod('c'), Pod('d')};
  jkds::util::resize(vec, 2);

  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec, std::vector<Pod>({Pod('a'), Pod('b')}));
}
