#pragma once

#include <string.h>
#include <cstdint>

namespace jkds::container {

  /***
   * SparseByteSet
   *
   * A simple data structure modeling an append-only set of bytes.
   * It may perform better than std::bitset<256>.
   *
   * Public methods:
   * - add(uint8_t)
   * - contains(uint8_t)
   * - reset()
   *
   * Performance concerns:
   * - This set never allocates.
   * - Every method runs in constant time (with a small constant factor)
   *
   * This data structure is inspired by "An Efficient Representation for Sparse Sets",
   * by Preston Briggs and Linda Torczon.
   */
  class SparseByteSet {
  public:
    // 256 is 2^8
    static constexpr uint16_t capacity = 256;

    SparseByteSet() : size_(0) {
    }

    /***
     * Add a new byte to the set.
     * Time: O(1), Space: O(1)
     */
    inline bool add(uint8_t byte) {
      bool result = !contains(byte);

      if (result) {
        // assuming size_ < capacity
        dense_[size_] = byte;
        sparse_[byte] = static_cast<uint8_t>(size_);
        size_++;
      }

      return result;
    }

    /***
     * Check whether a given byte is in the set.
     * Time: O(1), Space: O(1)
     */
    inline bool contains(uint8_t byte) const {
      return sparse_[byte] < size_ && dense_[sparse_[byte]] == byte;
    }

    /***
     * Reset the byte set.
     * Time: O(256), Space: O(1)
     */
    void reset() {
      size_ = 0;
      memset(sparse_, 0, sizeof(sparse_));
      memset(dense_, 0, sizeof(dense_));
    }

  private:
    // to avoid overflows in add(), size can't be uint8_t
    uint16_t size_;

    uint8_t sparse_[capacity];
    uint8_t dense_[capacity];
  };

}  // namespace jkds::container