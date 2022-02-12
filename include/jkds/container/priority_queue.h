#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../functional/zip.h"
#include "../util/range.h"
#include "heap.h"
#include "binary_heap.h"
#include "k_heap.h"

namespace jkds::container {

  /***
   * PriorityQueue
   *
   * A generic Priority Queue data structure based on a Heap.
   * KeyT is the type of the keys used to order the Heap.
   * ValueT is the type of the elements stored in the Heap underlying the Priority Queue.
   * The internal elements are allocated in an std::vector<ValueT> container.
   *
   * Public methods:
   * - size()
   * - empty()
   * - push(const KeyT&, const ValueT&)
   * - pop()
   * - update_key(const KeyT&, const ValueT&)
   * - key_at(const ValueT&)
   * - contains(const ValueT&)
   * - top()
   * - top_key_value()
   *
   * Performance concerns:
   * - The heapify_* operations are implemented iteratively.
   * - If IsHeap=true, the move constructor only requires O(1) time, as the given vector is
   * considered already a heap.
   */
  template <detail::heap_type HeapT, class HeapC, typename KeyT, typename ValueT,
            bool IsHeap = false,
            typename = std::enable_if<std::is_base_of<Heap<HeapT, ValueT, IsHeap>, HeapC>::value>>
  class PriorityQueue final : HeapC {
  private:
    using super = HeapC;
    using key_map_t = std::unordered_map<ValueT, KeyT>;
    using index_map_t = std::unordered_map<ValueT, std::size_t>;

    // keep track of the value of the keys of each node
    key_map_t key_map_;

    // keep track of the index of the keys of each node
    index_map_t index_map_;

    // Factory for key_map_
    template <typename KeyU, typename ValueU>
    [[nodiscard]] static auto build_key_map(std::vector<KeyU>&& keys,
                                            const std::vector<ValueU>& nodes) {
      assert(keys.size() == nodes.size());
      std::unordered_map<ValueU, KeyU> key_map(nodes.size());

      for (auto&& [key, node] : functional::zip(keys, nodes)) {
        key_map[node] = key;
      }

      return key_map;
    }

    // Factory for index_map_
    template <typename ValueU>
    [[nodiscard]] static auto build_index_map(const std::vector<ValueU>& nodes) noexcept {
      const std::size_t n = nodes.size();
      std::unordered_map<ValueU, std::size_t> index_map(n);
      auto indexes(util::range<size_t>(n));

      for (auto&& [node, index] : functional::zip(nodes, indexes)) {
        index_map[node] = index;
      }

      return index_map;
    }

  protected:
    void swap_nodes(std::size_t i, std::size_t j) noexcept {
      auto node_i = this->nodes_[i];
      auto node_j = this->nodes_[j];

      // swap the indexes first, then the nodes
      std::swap(index_map_[node_i], index_map_[node_j]);
      super::swap_nodes(i, j);
    }

  public:
    PriorityQueue() = delete;

    explicit PriorityQueue(const std::vector<KeyT>& keys, const std::vector<ValueT>& inputs) :
        super(inputs), key_map_(build_key_map(keys, inputs)), index_map_(build_key_map(inputs)) {
      super::heapify();
    }

    explicit PriorityQueue(std::vector<KeyT>&& keys, std::vector<ValueT>&& inputs) : super(std::move(inputs)), key_map_(build_key_map(std::move(keys), this->nodes_)), index_map_(build_index_map(this->nodes_)) {
      super::heapify();
    }

    /***
     * size
     *
     * Return the number of elements in the priority queue.
     * Time: O(1), Space: O(1)
     */
    [[nodiscard]] std::size_t size() const noexcept {
      return super::size();
    }

    /***
     * empty
     *
     * Return true when the priority queue is empty.
     * Time: O(1), Space: O(1)
     */
    [[nodiscard]] bool empty() const noexcept {
      return super::empty();
    }

    /***
     * push
     *
     * Add a new element to the priority queue, and associates the given key with it.
     * The given key is assumed to be unique w.r.t. the keys associated to the other
     * elements that may be pushed to the priority queue.
     * Time: O(log(n)) amortized if using a BinaryHeap, or O(K log_K(n)) amortized if using a KHeap.
     * Space: O(1) amortized.
     */
    void push(const KeyT& key, const ValueT& element) {
      const std::size_t index_to_fix = this->size();
      index_map_[element] = index_to_fix;
      key_map_[element] = key;

      super::push(element);
    }

    /***
     * pop
     *
     * Removes the top element (which must exist).
     * Time: O(log(n)) amortized if using a BinaryHeap, or O(K log_K(n)) amortized if using a KHeap.
     * Space: O(1) amortized.
     */
    void pop() {
      assert(this->size() > 0);

      const std::size_t index_to_remove = 0;
      const auto node = this->nodes_.at(index_to_remove);
      index_map_.erase(node);
      key_map_.erase(node);

      // replace root of the heap with the last element of the vector
      this->nodes_[0] = this->nodes_.back();

      // remove the last element of the vector
      this->nodes_.pop_back();

      // no need to recover the heap property if the heap is empty
      if (this->size() > 0) {
        const auto front = this->nodes_.front();
        index_map_[front] = 0;

        // the root violates the heap property. Let's fix that
        this->heapify_down(0);
      }
    }

    /***
     * update_key
     *
     * Update the key of an element in the priority queue.
     * The element must already be in the priority queue.
     * Time: O(log(n)) amortized if using a BinaryHeap, or O(K log_K(n)) amortized if using a KHeap.
     * Space: O(1) amortized.
     */
    void update_key(const KeyT& key, const ValueT& element) {
      const std::size_t index_to_fix = index_map_[element];
      key_map_[element] = key;

      // recover the heap property
      [this, index_to_fix]() constexpr {
        switch (HeapT) {
          case detail::heap_type::min_heap:
            // we're using a Min Heap, thus we are decreasing the key
            this->heapify_up(index_to_fix);
            break;
          case detail::heap_type::max_heap:
            // we're using a Max Heap, thus we are increasing the key
            this->heapify_down(index_to_fix);
            break;
        }
      }();
    }

    /***
     * key_at
     *
     * Return the key associated to the given element.
     * Time: O(1) amortized.
     * Space: O(1).
     */
    [[nodiscard]] const KeyT& key_at(const ValueT& element) const {
      return key_map_.at(element);
    }

    /***
     * contains
     *
     * Return true when the given element is in the priority queue.
     * Time: O(n), Space: O(1).
     */
    [[nodiscard]] bool contains(const ValueT& element) const noexcept {
      return index_map_.contains(element);
    }

    /***
     * top
     *
     * Return the top element of the priority queue.
     * Time: O(1), Space: O(1).
     */
    [[nodiscard]] const ValueT& top() const {
      return super::top();
    }

    /***
     * top_key_value
     *
     * Return the top (key, value) pair of the priority queue.
     * Time: O(1) amortized, Space: O(1).
     */
    [[nodiscard]] std::pair<const KeyT&, const ValueT&> top_key_value() const {
      const auto& top_value = super::top();
      const auto& top_key = key_at(top_value);
      return {top_key, top_value};
    }
  };

  /***
   * make_min_priority_queue
   *
   * Creates a Priority Queue based on a Min Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if keys/values are lvalues, O(1) if keys/values are rvalues.
   */
  template <bool IsHeap = false, typename KeyT, typename ValueT>
  auto make_min_priority_queue(std::vector<KeyT> keys = {}, std::vector<ValueT> values = {}) {
    constexpr auto heap_t = detail::heap_type::min_heap;
    return PriorityQueue<heap_t, BinaryHeap<heap_t, ValueT, IsHeap>, KeyT, ValueT, IsHeap>(
        std::move(keys), std::move(values));
  }

  /***
   * make_min_k_priority_queue
   *
   * Creates a Priority Queue based on a Min K-Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if keys/values are lvalues, O(1) if keys/values are rvalues.
   */
  template <std::size_t K, bool IsHeap = false, typename KeyT, typename ValueT>
  auto make_min_k_priority_queue(std::vector<KeyT> keys = {}, std::vector<ValueT> values = {}) {
    constexpr auto heap_t = detail::heap_type::min_heap;
    return PriorityQueue<heap_t, KHeap<heap_t, K, ValueT, IsHeap>, KeyT, ValueT, IsHeap>(
        std::move(keys), std::move(values));
  }

  /***
   * make_max_priority_queue
   *
   * Creates a Priority Queue based on a Max Binary Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if keys/values are lvalues, O(1) if keys/values are rvalues.
   */
  template <bool IsHeap = false, typename KeyT, typename ValueT>
  auto make_max_priority_queue(std::vector<KeyT> keys = {}, std::vector<ValueT> values = {}) {
    constexpr auto heap_t = detail::heap_type::max_heap;
    return PriorityQueue<heap_t, BinaryHeap<heap_t, ValueT, IsHeap>, KeyT, ValueT, IsHeap>(
        std::move(keys), std::move(values));
  }

  /***
   * make_max_k_priority_queue
   *
   * Creates a Priority Queue based on a Max K-Heap from a vector of elements.
   * Time: O(n) if IsHeap=true, O(1) if IsHeap=false.
   * Space: O(n) if keys/values are lvalues, O(1) if keys/values are rvalues.
   */
  template <std::size_t K, bool IsHeap = false, typename KeyT, typename ValueT>
  auto make_max_k_priority_queue(std::vector<KeyT> keys = {}, std::vector<ValueT> values = {}) {
    constexpr auto heap_t = detail::heap_type::max_heap;
    return PriorityQueue<heap_t, KHeap<heap_t, K, ValueT, IsHeap>, KeyT, ValueT, IsHeap>(
        std::move(keys), std::move(values));
  }

}  // namespace jkds::container
