#include <gtest/gtest.h>
#include <jkds/container/binary_heap.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;
using namespace jkds::container;

namespace {

  struct Pod {
    uint8_t value;

    Pod(uint8_t value) : value(value) {
    }

    bool operator==(const Pod& other) const {
      return value == other.value;
    }
  };

  class MaxBinaryHeapTest : public ::testing::Test {
  protected:
    MaxBinaryHeapTest() {
      sort(test_vector_sorted.begin(), test_vector_sorted.end(), greater<>());
      sort(test_vector_pod_sorted.begin(), test_vector_pod_sorted.end(), greater<>());
    }

    vector<uint8_t> test_vector = {30, 1, 50, 20, 40, 60, 100};
    vector<uint8_t> test_vector_sorted = test_vector;

    vector<shared_ptr<Pod>> test_vector_pod = {
        make_shared<Pod>(30), make_shared<Pod>(1),  make_shared<Pod>(50),
        make_shared<Pod>(40), make_shared<Pod>(60), make_shared<Pod>(100),
    };
    vector<shared_ptr<Pod>> test_vector_pod_sorted = test_vector_pod;
    vector<uint8_t> already_max_seed = {8, 7, 6, 5, 4, 3, 2, 1, 0};
  };

}  // namespace

namespace std {
  template <>
  struct greater<shared_ptr<Pod>> {
    bool operator()(const shared_ptr<Pod> lhs, const shared_ptr<Pod> rhs) const {
      return lhs->value > rhs->value;
    }
  };

}  // namespace std

TEST_F(MaxBinaryHeapTest, empty) {
  auto empty_max_heap = make_max_heap<false, uint8_t>();
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

TEST_F(MaxBinaryHeapTest, empty_pod) {
  auto empty_max_heap_pod = make_max_heap<false, shared_ptr<Pod>>();
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

TEST_F(MaxBinaryHeapTest, full) {
  auto max_heap = make_max_heap(test_vector);
  ASSERT_FALSE(max_heap.empty());
  ASSERT_EQ(max_heap.size(), test_vector.size());

  for (const auto& v : test_vector_sorted) {
    const auto x = max_heap.top();
    ASSERT_EQ(x, v);
    max_heap.pop();
  }
}

TEST_F(MaxBinaryHeapTest, already_heap) {
  auto already_max_heap = make_max_heap<true>(already_max_seed);
  ASSERT_FALSE(already_max_heap.empty());
  ASSERT_EQ(already_max_heap.size(), already_max_seed.size());

  for (const auto& v : already_max_seed) {
    const auto x = already_max_heap.top();
    ASSERT_EQ(x, v);
    already_max_heap.pop();
  }
}

TEST_F(MaxBinaryHeapTest, init_copy) {
  std::vector<uint8_t> vector{0, 1, 2, 3};
  auto heap = make_max_heap(vector);
  ASSERT_EQ(heap.size(), 4);
  ASSERT_EQ(vector.size(), 4);
}

TEST_F(MaxBinaryHeapTest, init_move) {
  std::vector<uint8_t> vector{0, 1, 2, 3};
  auto heap = make_max_heap(std::move(vector));
  ASSERT_EQ(heap.size(), 4);
  ASSERT_EQ(vector.size(), 0);
}
