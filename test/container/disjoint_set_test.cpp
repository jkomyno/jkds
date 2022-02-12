#include <gtest/gtest.h>
#include <jkds/container/disjoint_set.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;
using namespace jkds::container;

namespace {

  class DisjointSetTest : public ::testing::Test {
  };

  template <typename T>
  std::vector<std::vector<T>> sorted_sets(DisjointSet<T>& ds) {
    std::vector<std::vector<T>> result;

    for (auto [_, set] : ds.get_sets()) {
      std::sort(set.begin(), set.end());
      result.push_back(set);
    }

    std::sort(result.begin(), result.end(), [](const std::vector<T>& a, const std::vector<T>& b) {
      return a[0] < b[0];
    });
    return result;
  }

}  // namespace

TEST_F(DisjointSetTest, empty) {
  DisjointSet<char> ds{{}};
  ASSERT_EQ(ds.add('a'), 0);
  ASSERT_EQ(ds.add('b'), 1);
  ASSERT_EQ(ds.add('c'), 2);
  ASSERT_EQ(ds.add('d'), 3);
  ASSERT_EQ(ds.add('e'), 4);
  ASSERT_EQ(ds.add('f'), 5);
  ASSERT_EQ(ds.add('g'), 6);
  using sets_t = std::vector<std::vector<char>>;

  ASSERT_EQ(sorted_sets(ds), sets_t({{'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}, {'g'}}));

  ds.unite('a', 'b');
  ds.unite('c', 'd');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b'}, {'c', 'd'}, {'e'}, {'f'}, {'g'}}));

  ds.unite('a', 'd');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd'}, {'e'}, {'f'}, {'g'}}));

  ds.unite('b', 'c');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd'}, {'e'}, {'f'}, {'g'}}));

  ds.unite('e', 'f');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd'}, {'e', 'f'}, {'g'}}));

  ds.unite('c', 'f');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd', 'e', 'f'}, {'g'}}));

  ds.unite('g', 'd');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd', 'e', 'f', 'g'}}));
}

TEST_F(DisjointSetTest, full) {
  DisjointSet<char> ds{{'a', 'b', 'c', 'd', 'e'}};
  using sets_t = std::vector<std::vector<char>>;

  ASSERT_EQ(sorted_sets(ds), sets_t({{'a'}, {'b'}, {'c'}, {'d'}, {'e'}}));

  ds.unite('a', 'b');
  ds.unite('c', 'd');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b'}, {'c', 'd'}, {'e'}}));

  ASSERT_EQ(ds.add('f'), 5);
  ASSERT_EQ(ds.add('g'), 6);
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b'}, {'c', 'd'}, {'e'}, {'f'}, {'g'}}));

  ds.unite('a', 'd');
  ds.unite('c', 'e');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd', 'e'}, {'f'}, {'g'}}));

  ds.unite('e', 'f');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd', 'e', 'f'}, {'g'}}));

  ds.unite('g', 'd');
  ASSERT_EQ(sorted_sets(ds), sets_t({{'a', 'b', 'c', 'd', 'e', 'f', 'g'}}));
}
