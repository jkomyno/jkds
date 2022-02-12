#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

namespace jkds::container {

  namespace detail {
    enum class heap_type { min_heap, max_heap };
  }

  /***
   * Heap
   *
   * A generic Heap abstract data structure, where T is the type of the data to store in the heap.
   * The internal elements are allocated in an std::vector<T> container.
   *
   * Public methods:
   * - size()
   * - empty()
   * - top()
   * - pop()
   * - push(const T&)
   *
   * Performance concerns:
   * - The heapify_* operations are implemented iteratively.
   * - If IsHeap=true, the move constructor only requires O(1) time, as the given vector is
   * considered already a heap.
   */
  template <detail::heap_type HeapT, typename T, bool IsHeap = false>
  class Heap {
  protected:
    using Compare =
        std::conditional_t<HeapT == detail::heap_type::min_heap, std::greater<T>, std::less<T>>;

    Compare comp_;
    std::vector<T> nodes_;

    explicit Heap(const std::vector<T>& inputs) noexcept : nodes_(inputs) {
    }

    explicit Heap(std::vector<T>&& inputs) noexcept : nodes_(std::move(inputs)) {
    }

    // return the parent of nodes_[i]
    [[nodiscard]] virtual std::size_t parent(std::size_t i) const noexcept = 0;

    /***
     * Given an index of a misplaced node to fix, bubble the node down to recover the
     * heap properties.
     * Time: O(log n), Space: O(1)
     */
    virtual void heapify_down(std::size_t index_to_fix) noexcept = 0;

    virtual void swap_nodes(std::size_t i, std::size_t j) noexcept {
      std::swap(nodes_.at(i), nodes_.at(j));
    }

    /***
     * Given an index of a misplaced node to fix, bubble the node up to recover the
     * heap properties.
     * Time: O(log n), Space: O(1)
     */
    void heapify_up(const std::size_t index_to_fix) noexcept {
      std::size_t i = index_to_fix;

      while (i > 0 && comp_(nodes_.at(parent(i)), nodes_.at(i))) {
        const auto p = parent(i);
        swap_nodes(i, p);
        i = p;
      }
    }

    /***
     * Rearrange each node to form a heap.
     * Time: O(n), Space: O(1)
     */
    virtual void build_heap() noexcept = 0;

  public:
    Heap() = delete;

    // virtual destructor because this is a base class
    virtual ~Heap() = default;

    /***
     * Build the heap only if the vector received in the constructor wasn't already a valid heap.
     * Checking whether this is the case is up to the user: we trust them when IsHeap == true.
     */
    void heapify() noexcept {
      if constexpr (!IsHeap) {
        build_heap();
      }
    }

    // return the number of elements in the heap
    [[nodiscard]] size_t size() const noexcept {
      return nodes_.size();
    }

    // return true iff the heap is empty
    [[nodiscard]] bool empty() const noexcept {
      return size() == 0;
    }

    // return ths top element of the heap (which must exist)
    [[nodiscard]] const T& top() const {
      return nodes_.at(0);
    }

    // remove the top element of the heap (which must exist)
    void pop() {
      // replace root of the heap with the last element of the vector
      nodes_.at(0) = nodes_.back();

      // remove the last element of the vector
      nodes_.pop_back();

      // the root may violate the heap property, so it must be fixed
      heapify_down(0);
    }

    // add a new element to the heap
    template <class... Args>
    void push(Args&&... args) {
      const size_t index_to_fix = size();

      // insert new node at the end of the vector
      nodes_.emplace_back(std::forward<Args>(args)...);

      // the last leaf may violate the heap property, so it must be fixed
      heapify_up(index_to_fix);
    }
  };
}  // namespace jkds::container
