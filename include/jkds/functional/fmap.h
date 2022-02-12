#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace jkds::functional {

  /***
   * fmap
   * 
   * Higher-order function that applies a function to each element of a range
   * and returns a vector of the results.
   * Time: O(n), assuming constant time for the function invocation.
   * Space: O(n).
   */
  template <typename F, typename Iter>
  [[nodiscard]] auto fmap(F&& f, Iter first, Iter last)
      -> std::vector<decltype(f(std::declval<typename Iter::value_type>()))> {
    using T = decltype(f(std::declval<typename Iter::value_type>()));

    // pre-allocate the necessary memory, avoiding default constructors.
    std::vector<T> out;
    out.reserve(std::distance(first, last));

    std::transform(first, last, std::back_inserter(out), std::forward<F>(f));
    return out;
  }

  /***
   * fmap
   * 
   * Higher-order function that applies a function to each element of an iterable container
   * and returns a vector of the results.
   * Time: O(n), assuming constant time for the function invocation.
   * Space: O(n).
   */
  template <typename F, typename ContT>
  [[nodiscard]] auto fmap(F&& f, const ContT& container)
      -> std::vector<decltype(f(std::declval<typename ContT::value_type>()))> {
    return fmap(std::forward<F>(f), std::begin(container), std::end(container));
  }

}  // namespace jkds::functional
