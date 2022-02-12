#pragma once

#include <algorithm>
#include <iterator>

namespace jkds::util {

  /***
   * resize
   * 
   * Equivalent of container::resize(amount) that doesn't need container::value_type to be
   * default-constructible.
   * Time: O(n), Space: O(1)
   */
  template <typename Container, typename T = typename Container::value_type>
  void shift_to_value(Container& container, const T& value) {
    auto first = std::begin(container);
    auto last = std::end(container);
    auto pos_it = std::find(first, last, value);
    std::rotate(first, pos_it, last);
  }
}
