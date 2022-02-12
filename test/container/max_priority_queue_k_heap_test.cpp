#include <gtest/gtest.h>
#include <jkds/container/priority_queue.h>
#include <jkds/functional/zip.h>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;
using namespace jkds::container;
using namespace jkds::functional;

namespace {

  struct Pod {
    char value;

    Pod(char value) : value(value) {
    }

    bool operator==(const Pod& other) const {
      return value == other.value;
    }
  };

  class MaxPQKHeapTest : public ::testing::Test {
  protected:
  };

}  // namespace

namespace std {
  template <>
  struct less<shared_ptr<Pod>> {
    bool operator()(const shared_ptr<Pod>& lhs, const shared_ptr<Pod>& rhs) const {
      return std::less<>{}(lhs->value, rhs->value);
    }
  };

  template <>
  struct hash<shared_ptr<Pod>> {
    size_t operator()(const shared_ptr<Pod>& p) const {
      return hash<char>()(p->value);
    }
  };

}  // namespace std

TEST_F(MaxPQKHeapTest, full) {
  // Associate (5 -> 'm'), (4, 'i'), (1, 'n'), (3, 'h'), (6, 'e'), (0, 'a'), (2, 'p')
  vector<uint8_t> keys{5, 4, 1, 3, 6, 0, 2};
  vector<char> values{'m', 'i', 'n', 'h', 'e', 'a', 'p'};

  auto max_pq(make_max_k_priority_queue<4>(keys, values));

  EXPECT_FALSE(max_pq.empty());
  EXPECT_EQ(max_pq.size(), 7);

  vector<pair<uint8_t, char>> expected_top_key_values{{2, 'p'}, {1, 'n'}, {5, 'm'}, {4, 'i'},
                                                      {3, 'h'}, {6, 'e'}, {0, 'a'}};

  for (auto&& [key, value] : expected_top_key_values) {
    EXPECT_EQ(max_pq.top(), value);
    EXPECT_EQ(max_pq.top_key_value().first, key);
    EXPECT_EQ(max_pq.top_key_value().second, value);
    max_pq.pop();
  }

  EXPECT_TRUE(max_pq.empty());
  EXPECT_EQ(max_pq.size(), 0);
}

TEST_F(MaxPQKHeapTest, full_pod) {
  // Associate (5 -> 'm'), (4, 'i'), (1, 'n'), (3, 'h'), (6, 'e'), (0, 'a'), (2, 'p')
  vector<uint8_t> keys{5, 4, 1, 3, 6, 0, 2};
  auto m_pod = make_shared<Pod>('m');
  auto i_pod = make_shared<Pod>('i');
  auto n_pod = make_shared<Pod>('n');
  auto h_pod = make_shared<Pod>('h');
  auto e_pod = make_shared<Pod>('e');
  auto a_pod = make_shared<Pod>('a');
  auto p_pod = make_shared<Pod>('p');
  vector<shared_ptr<Pod>> values{
    m_pod, i_pod, n_pod, h_pod, e_pod, a_pod, p_pod
  };

  auto max_pq(make_max_k_priority_queue<4>(keys, values));

  EXPECT_FALSE(max_pq.empty());
  EXPECT_EQ(max_pq.size(), 7);

  vector<pair<uint8_t, shared_ptr<Pod>>> expected_top_key_values{{2, p_pod}, {1, n_pod}, {5, m_pod}, {4, i_pod},
                                                                 {3, h_pod}, {6, e_pod}, {0, a_pod}};

  for (auto&& [key, value] : expected_top_key_values) {
    EXPECT_EQ(max_pq.top(), value);
    EXPECT_EQ(max_pq.top_key_value().first, key);
    EXPECT_EQ(max_pq.top_key_value().second, value);
    max_pq.pop();
  }

  EXPECT_TRUE(max_pq.empty());
  EXPECT_EQ(max_pq.size(), 0);
}
