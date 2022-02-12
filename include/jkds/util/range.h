#include <numeric>
#include <vector>

namespace jkds::util {

  /***
   * range 
   * 
   * Utility that generates a sequential range of values of a specific size.
   * If the start value is not specified, the range starts from 0.
   */
  template<typename T>
  auto range(std::size_t size, T start = T(0)) -> std::vector<T> {
    std::vector<T> vec(size);
    std::iota(vec.begin(), vec.end(), start);
    return vec;
  }
} // namespace jkds::util
