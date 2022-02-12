#pragma once

#include <type_traits>
#include <vector>

#include "heap.h"

namespace jkds::container {

  /***
   * BinaryHeap
   *
   * A generic Binary Heap data structure, where T is the type of the data to store in the heap.
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
   * If T is not a standard data type, a custom implementation of std::greater<T> must be provided
   * for creating a max heap, and a custom implementation of std::less<T> must be provided for
   * creating a min heap.
   */
  template <detail::heap_type HeapT, typename T, bool IsHeap = false>
  class BinaryHeap : public Heap<HeapT, T, IsHeap> {
  private:
    using super = Heap<HeapT, T, IsHeap>;

    // returns the left child of nodes[i]
    [[nodiscard]] static std::size_t left(const std::size_t i) noexcept {
      return (i << 1) + 1;
    }

    // returns the right child of nodes[i]
    [[nodiscard]] static std::size_t right(const std::size_t i) noexcept {
      return (i << 1) + 2;
    }

    // returns true iff the nodes[i] is a leaf
    [[nodiscard]] bool is_leaf(const size_t i) const noexcept {
      return i >= (this->size() >> 1) + 1;
    }

  protected:
    // return the parent of nodes[i]
    [[nodiscard]] std::size_t parent(const std::size_t i) const noexcept override final {
      return (i - 1) >> 1;
    }

    /***
     * Given an index of a misplaced node to fix, bubble the node down to recover the
     * heap properties.
     * Time: O(log n), Space: O(1)
     */
    void heapify_down(const std::size_t index_to_fix) noexcept override {
      const std::size_t len = this->size();
      std::size_t i = index_to_fix;

      while (!is_leaf(i)) {
        const auto l = left(i);
        const auto r = right(i);

        // comp_est is the biggest element in a Max Heap,
        // or the smallest element in a Min Heap
        std::size_t comp_est = i;

        if (l < len && this->comp_(this->nodes_[i], this->nodes_[l])) {
          comp_est = l;
        }

        if (r < len && this->comp_(this->nodes_[comp_est], this->nodes_[r])) {
          comp_est = r;
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
      for (std::size_t i = (this->size() >> 1) + 1; i > 0; --i) {
        heapify_down(i - 1);
      }
    }

  public:
    BinaryHeap() = delete;

    explicit BinaryHeap(const std::vector<T>& inputs) noexcept : super(inputs) {

    }

    explicit BinaryHeap(std::vector<T>&& inputs) noexcept : super(std::move(inputs)) {

    }
  };

  /***
   * make_min_heap
   *
   * Creates a Min Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if inputs is an lvalue, O(1) if inputs is an rvalue.
   */
  template <bool IsHeap = false, typename T>
  auto make_min_heap(std::vector<T> inputs = {}) noexcept {
    auto min_heap(BinaryHeap<detail::heap_type::min_heap, T, IsHeap>(std::move(inputs)));
    min_heap.heapify();
    return min_heap;
  }

  /***
   * make_max_heap
   *
   * Creates a Max Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if inputs is an lvalue, O(1) if inputs is an rvalue.
   */
  template <bool IsHeap = false, typename T>
  auto make_max_heap(std::vector<T> inputs = {}) noexcept {
    auto max_heap(BinaryHeap<detail::heap_type::max_heap, T, IsHeap>(std::move(inputs)));
    max_heap.heapify();
    return max_heap;
  }
}  // namespace jkds::container
