#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

#include "../functional/fmap.h"
#include "../functional/zip.h"
#include "../util/range.h"

namespace jkds::container {

  /***
   * DisjointSet
   *
   * A Disjoint Set data structure (also known as Union-Find), where T is the type of the
   * data to store in non-overlapping sets.
   * The simplest implementations only support integer elements. However, we support any type of elements;
   * the elements are mapped to an std::size_t index internally. 
   * 
   * The internal elements are stored in a std::vector<*> container, and the mapping between
   * elements an their indexes in the vector are stored in a std::unordered_map<T, std::size_t> container.
   * Hence, an implementation of std::hash<T> is required.
   * 
   * DisjointSet is optimized, as it implements the union-by-rank policy paired with path-splitting compression,
   * which results in almost constant time complexity for every method. 
   * 
   * Public methods:
   * - add(const T&)
   * - unite(const T&, const T&)
   * - are_connected(const T&, const T&)
   * - get_sets()
   */
  template <typename T>
  class DisjointSet {
  private:
    struct Node {
      std::size_t parent;
      std::size_t rank = 0;

      Node(std::size_t parent) : parent(parent) {
      }
    };
    std::vector<Node> nodes_;
    std::unordered_map<T, std::size_t> index_map_;

    // initialize every item as the parent of itself with rank 0
    [[nodiscard]] static std::vector<Node> init_nodes(std::size_t size) noexcept {
      auto parents(jkds::util::range<std::size_t>(size));
      return jkds::functional::fmap(
          [](std::size_t parent) {
            return Node(parent);
          },
          parents);
    }

    // initialize the index map in sequential order, starting from 0
    template <typename V>
    [[nodiscard]] static std::unordered_map<V, std::size_t> init_index_map(
        std::vector<V>&& inputs) noexcept {
      const auto size = inputs.size();
      std::unordered_map<V, std::size_t> index_map;
      index_map.reserve(size);

      for (auto&& [x, i] : jkds::functional::zip(inputs, jkds::util::range<std::size_t>(size))) {
        index_map.emplace(x, i);
      }

      return index_map;
    }

    // Returns of the representative set containing the element indexed by the given index
    [[nodiscard]] std::size_t find(std::size_t index) noexcept {
      while (index != nodes_[index].parent) {
        // skip parent and point to grandparent
        const auto grandparent = nodes_[nodes_[index].parent].parent;
        nodes_[index].parent = grandparent;
        index = grandparent;
      }

      return index;
    }

  public:
    DisjointSet() = delete;

    explicit DisjointSet(const std::vector<T>& inputs) noexcept :
        nodes_(init_nodes(inputs.size())), index_map_(init_index_map(inputs)) {
    }

    explicit DisjointSet(std::vector<T>&& inputs) noexcept :
        nodes_(init_nodes(inputs.size())), index_map_(init_index_map(std::move(inputs))) {
    }

    /***
     * add
     * 
     * Add a new entry to the disjoint set, returning the index of the resulting node.
     * Time: O(1) amortized, Space: O(1)
     */
    std::size_t add(const T& x) {
      const auto i = nodes_.size();
      nodes_.emplace_back(i);
      const auto&& [it, ok] = index_map_.emplace(x, i);
      assert(ok);
      return it->second;
    }

    /***
     * unite
     * 
     * Merge two dynamic sets which contain the x and y elements, respectively, into a new set
     * that is the union of the two sets.
     * x is assumed to be different than y.
     * Time: O(lg^* n) amortized, Space: O(1) 
     */
    void unite(const T& x, const T& y) noexcept {
      auto i = find(index_map_.at(x));
      auto j = find(index_map_.at(y));

      if (i == j) {
        return;
      }

      // if two sets are united and have different ranks,
      // the resulting set's rank is the larger of the two
      if (nodes_[i].rank < nodes_[j].rank) {
        std::swap(i, j);
      }

      nodes_[j].parent = i;

      // if two sets are united and have the same rank,
      // the resulting representative set's rank is one unit larger
      if (nodes_[i].rank == nodes_[j].rank) {
        ++(nodes_[j].rank);
      }
    }

    /***
     * are_connected
     * 
     * Return true if and only if the given two elements are in the same representative set.
     * Time: O(lg^* n) amortized, Space: O(1) 
     */
    [[nodiscard]] bool are_connected(const T& x, const T& y) noexcept {
      return find(index_map_.at(x)) == find(index_map_.at(y));
    }

    /***
     * get_sets
     * 
     * Snapshot the representative sets.
     * Time: O(n + lg^* n), Space: O(1) 
     */
    [[nodiscard]] auto get_sets() noexcept {
      std::unordered_map<std::size_t, std::vector<T>> sets;
      for (auto&& [x, index] : index_map_) {
        sets[find(index)].emplace_back(x);
      }
      return sets;
    }
  };
}  // namespace jkds::container
