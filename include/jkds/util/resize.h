#pragma once

#include <cstddef>
#include <iterator>

namespace jkds::util {

  /***
   * resize
   * 
   * Equivalent of container::resize(amount) that doesn't need container::value_type to be
   * default-constructible.
   * Time: O(std::end(container) - amount), Space: O(1)
   */
  template <typename Container>
  void resize(Container&& container, std::size_t amount) {
    container.erase(std::end(container) - amount, std::end(container));
  }
}
