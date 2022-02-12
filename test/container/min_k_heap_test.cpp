#include <gtest/gtest.h>
#include <jkds/container/k_heap.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;
using namespace jkds::container;

namespace {

  struct Pod {
    uint8_t value;

    explicit Pod(uint8_t value) : value(value) {
    }

    bool operator==(const Pod& other) const {
      return value == other.value;
    }
  };

  class MinKHeapTest : public ::testing::Test {
  protected:
    MinKHeapTest() {
      sort(test_vector_sorted.begin(), test_vector_sorted.end(), less<>());
      sort(test_vector_pod_sorted.begin(), test_vector_pod_sorted.end(), less<>());
    }

    vector<uint8_t> test_vector = {30, 1, 50, 20, 40, 60, 100};
    vector<uint8_t> test_vector_sorted = test_vector;

    vector<shared_ptr<Pod>> test_vector_pod = {
        make_shared<Pod>(30), make_shared<Pod>(1),  make_shared<Pod>(50),
        make_shared<Pod>(40), make_shared<Pod>(60), make_shared<Pod>(100),
    };
    vector<shared_ptr<Pod>> test_vector_pod_sorted = test_vector_pod;
    vector<uint8_t> already_max_seed = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  };

}  // namespace

namespace std {
  template <>
  struct less<shared_ptr<Pod>> {
    bool operator()(const shared_ptr<Pod> lhs, const shared_ptr<Pod> rhs) const {
      return lhs->value > rhs->value;
    }
  };

}  // namespace std

TEST_F(MinKHeapTest, empty) {
  auto empty_max_heap = make_min_k_heap<4, false, uint8_t>();
  ASSERT_TRUE(empty_max_heap.empty());

  for (const auto& v : test_vector) {
    empty_max_heap.push(v);
  }

  ASSERT_FALSE(empty_max_heap.empty());
  ASSERT_EQ(empty_max_heap.size(), test_vector.size());

  for (const auto& v : test_vector_sorted) {
    const auto x = empty_max_heap.top();
    ASSERT_EQ(x, v);
    empty_max_heap.pop();
  }
}

TEST_F(MinKHeapTest, empty_pod) {
  auto empty_max_heap_pod = make_min_k_heap<4, false, shared_ptr<Pod>>();
  ASSERT_TRUE(empty_max_heap_pod.empty());

  for (const auto& v : test_vector_pod) {
    empty_max_heap_pod.push(v);
  }

  ASSERT_FALSE(empty_max_heap_pod.empty());
  ASSERT_EQ(empty_max_heap_pod.size(), test_vector_pod.size());

  for (const auto v : test_vector_pod_sorted) {
    const auto x = empty_max_heap_pod.top();
    ASSERT_EQ(*x, *v);
    empty_max_heap_pod.pop();
  }
}

TEST_F(MinKHeapTest, full) {
  auto max_heap = make_min_k_heap<4>(test_vector);
  ASSERT_FALSE(max_heap.empty());
  ASSERT_EQ(max_heap.size(), test_vector.size());

  for (const auto& v : test_vector_sorted) {
    const auto x = max_heap.top();
    ASSERT_EQ(x, v);
    max_heap.pop();
  }
}

TEST_F(MinKHeapTest, already_heap) {
  auto already_max_heap = make_min_k_heap<4, true>(already_max_seed);
  ASSERT_FALSE(already_max_heap.empty());
  ASSERT_EQ(already_max_heap.size(), already_max_seed.size());

  for (const auto& v : already_max_seed) {
    const auto x = already_max_heap.top();
    ASSERT_EQ(x, v);
    already_max_heap.pop();
  }
}

TEST_F(MinKHeapTest, init_copy) {
  std::vector<uint8_t> vector{0, 1, 2, 3};
  auto heap = make_min_k_heap<4>(vector);
  ASSERT_EQ(heap.size(), 4);
  ASSERT_EQ(vector.size(), 4);
}

TEST_F(MinKHeapTest, init_move) {
  std::vector<uint8_t> vector{0, 1, 2, 3};
  auto heap = make_min_k_heap<4>(std::move(vector));
  ASSERT_EQ(heap.size(), 4);
  ASSERT_EQ(vector.size(), 0);
}
