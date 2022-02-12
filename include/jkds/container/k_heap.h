#pragma once

#include <type_traits>
#include <vector>

#include "heap.h"

namespace jkds::container {

  /***
   * KHeap
   *
   * A generic K-ary Heap data structure, where T is the type of the data to store in the heap.
   * The internal elements are allocated in an std::vector<T> container.
   *
   * Public methods:
   * - size()
   * - empty()
   * - top()
   * - pop()
   * - push(const T&)
   *
   * This class shouldn't be invoked directly.
   * Instead, the factories make_min_heap and make_max_heap should be used.
   * K indicates the Heap arity, and it must be bigger than 2. We suggest using powers of 2 (e.g. 4,
   * 8, ...). If T is not a standard data type, a custom implementation of std::greater<T> must be
   * provided for creating a max heap, and a custom implementation of std::less<T> must be provided
   * for creating a min heap.
   */
  template <detail::heap_type HeapT, std::size_t K, typename T, bool IsHeap = false,
            typename = typename std::enable_if<(K > 2 && K <= 64)>::type>
  class KHeap : public Heap<HeapT, T, IsHeap> {
  private:
    using super = Heap<HeapT, T, IsHeap>;

    // returns the j-th child of the i-th node.
    [[nodiscard]] static size_t child(const size_t i, const size_t j) noexcept {
      return (K * i) + j + 1;
    }

    // returns true iff the i-th is a leaf
    [[nodiscard]] bool is_leaf(const size_t i) const noexcept {
      return i > ((this->size() - 2) / K);
    }

  protected:
    // return the parent of nodes[i]
    [[nodiscard]] std::size_t parent(const std::size_t i) const noexcept override final {
      return (i - 1) / K;
    }

    /***
     * Given an index of a misplaced node to fix, bubble the node down to recover the
     * heap properties.
     * Time: O(log_K n), Space: O(1)
     */
    void heapify_down(const std::size_t index_to_fix) noexcept override {
      const std::size_t len = this->size();
      std::size_t i = index_to_fix;

      // comp_est is the biggest element in a Max Heap,
      // or the smallest element in a Min Heap
      std::size_t comp_est = i;

      while (!is_leaf(i)) {
        // Time: O(K)
        for (std::size_t j = 0; j < K; ++j) {
          const auto son = child(i, j);
          if (son < len && this->comp_(this->nodes_[comp_est], this->nodes_[son])) {
            comp_est = son;
          }
        }

        if (i == comp_est) {
          return;
        }

        this->swap_nodes(i, comp_est);
        i = comp_est;
      }
    }

    /***
     * Rearrange each node to form a heap.
     * Time: O(n), Space: O(1)
     */
    void build_heap() noexcept override final {
      for (std::size_t i = (this->size() / K) + 1; i > 0; --i) {
        heapify_down(i - 1);
      }
    }

  public:
    KHeap() = delete;

    explicit KHeap(const std::vector<T>& inputs) noexcept : super(inputs) {

    }

    explicit KHeap(std::vector<T>&& inputs) noexcept : super(std::move(inputs)) {

    }
  };

  /***
   * make_min_heap
   *
   * Creates a Min Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if inputs is an lvalue, O(1) if inputs is an rvalue.
   */
  template <std::size_t K, bool IsHeap = false, typename T>
  auto make_min_k_heap(std::vector<T> inputs = {}) noexcept {
    auto min_k_heap(KHeap<detail::heap_type::min_heap, K, T, IsHeap>(std::move(inputs)));
    min_k_heap.heapify();
    return min_k_heap;
  }

  /***
   * make_max_heap
   *
   * Creates a Min Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if inputs is an lvalue, O(1) if inputs is an rvalue.
   */
  template <std::size_t K, bool IsHeap = false, typename T>
  auto make_max_k_heap(std::vector<T> inputs = {}) noexcept {
    auto max_k_heap(KHeap<detail::heap_type::max_heap, K, T, IsHeap>(std::move(inputs)));
    max_k_heap.heapify();
    return max_k_heap;
  }
}  // namespace jkds::container
