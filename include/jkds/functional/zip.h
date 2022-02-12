#pragma once

#include <functional>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

namespace jkds::functional {

  namespace detail {

    // select_access_type_for returns the access type for the dereferenced values of iterators.
    // Special care is taken for std::vector<bool>, which cannot return a reference, but only
    // values.
    template <typename Iter>
    using select_access_type_for =
        std::conditional_t<std::is_same_v<Iter, std::vector<bool>::iterator> ||
                               std::is_same_v<Iter, std::vector<bool>::const_iterator>,
                           typename Iter::value_type, typename Iter::reference>;

    // any_match_impl applies a fold expression to an index sequence, allowing the elementwise
    // comparison of tuple values. The function returns true if of the tuple elements are equal.
    template <typename... Args, std::size_t... Idx>
    auto any_match_impl(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs,
                        std::index_sequence<Idx...>) -> bool {
      auto result = false;
      result = (... | (std::get<Idx>(lhs) == std::get<Idx>(rhs)));
      return result;
    }

    // any_match returns true when any elementwise match in the given two tuples occurs.
    // The tuples are forwarded along with a generated index sequence matching the size
    // of the tuples. This is used to make sure that the iteration terminates as soon as
    // any of the iterators reaches their end position.
    template <typename... Args>
    auto any_match(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs) -> bool {
      return any_match_impl(lhs, rhs, std::index_sequence_for<Args...>{});
    }

    // select_iterator_for returns a const_iterator in case the given type T is a const reference,
    // otherwise it returns a standard iterator.
    template <typename T>
    using select_iterator_for = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>,
                                                   typename std::decay_t<T>::const_iterator,
                                                   typename std::decay_t<T>::iterator>;

    // The zip_iterator class holds the iterators for the actual ranges being iterated over.
    template <typename... Iters>
    class zip_iterator {
    private:
      std::tuple<Iters...> iters_;

    public:
      using value_type = std::tuple<select_access_type_for<Iters>...>;

      zip_iterator() = delete;

      zip_iterator(Iters&&... iters) : iters_{std::forward<Iters>(iters)...} {
      }

      zip_iterator& operator++() {
        std::apply(
            [](auto&&... args) {
              (++args, ...);
            },
            iters_);
        return *this;
      }

      zip_iterator operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator!=(zip_iterator const& other) const {
        return !(*this == other);
      }

      bool operator==(zip_iterator const& other) const {
        return detail::any_match(iters_, other.iters_);
      }

      auto operator*() -> value_type {
        return std::apply(
            [](auto&&... args) {
              return value_type(*args...);
            },
            iters_);
      }
    };

    template <typename... T>
    class zipper {
    private:
      std::tuple<T...> args_;

    public:
      using zip_t = zip_iterator<select_iterator_for<T>...>;

      template <typename... Args>
      zipper(Args&&... args) : args_{std::forward<Args>(args)...} {
      }

      auto begin() -> zip_t {
        return std::apply(
            [](auto&&... args) {
              return zip_t(std::begin(args)...);
            },
            args_);
      }

      auto end() -> zip_t {
        return std::apply(
            [](auto&&... args) {
              return zip_t(std::end(args)...);
            },
            args_);
      }
    };
  }  // namespace detail

  /**
   * zip
   * 
   * Utility to create a zipper to iterate over the multiple given ranges.
   * 
   * Example usage:
   * auto first = std::vector<int>{1, 2, 3, 4};
   * auto second = std::vector<int>{5, 6, 7};
   * for (auto&& [a, b] : zip(first, second)) {
   *   std::cout << a << ' ' << b << ';' << ' ';
   * }
   * 
   * It should print "1 5; 2 6; 3 7;".
   */
  template <typename ... T>
  auto zip(T&&... args) -> detail::zipper<T...> {
    return detail::zipper<T...>{std::forward<T>(args)...};
  }

}  // namespace jkds::functional
