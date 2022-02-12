# jkds

[![Build](https://github.com/jkomyno/jkds/workflows/Build/badge.svg)](https://github.com/jkomyno/jkds/actions?query=workflow:Build)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/0a2fdaf68ade4424a8fa9922b624ed27)](https://www.codacy.com/gh/jkomyno/jkds/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jkomyno/jkds&amp;utm_campaign=Badge_Grade)

`jkds` is a modern header-only C++20 library that complements the standard library.
It provides generic atypical data structures, ergonomic functional programming abstractions, and then some. 

This library is the result of several projects Alberto Schiabel has been involved into his student years.
Even though it's currently used for competitive programming and academic purposes, `jkds`
aims to be a helpful resource to showcase how to implement data structures in a readable, generic, and composable fashion. Thus, we invite the users to read the source code and help us improve it by contributing to this repository.

# Build and Test

This project supports [CMake](https://cmake.org/) 3.20+.
Unit tests are run using the [GoogleTest](https://github.com/google/googletest) suite.

```
git clone https://github.com/jkomyno/jkds
cd jkds
./build.sh
```

# Status

`jkds` is currently a work-in-progress, but it's already suitable for production.

# Contents

The `jkds` library provides the following namespaces:
- `jkds::container`: custom containers alternative to the STL library;
- `jkds::functional`: abstract utilities to use functional programming directives in modern C++;
- `jkds::util`: general purpose utilities.

## jkds::container

The custom container classes are defined in [`./include/jkds/container`](`./include/jkds/container`).

### BinaryHeap

The `BinaryHeap<HeapT, T, IsHeap=false>` class (defined in [`binary_heap.h`](`./include/jkds/container/binary_heap.h`))
is a binary heap data structure.
It's fully generic, not only in terms of the underlying type `T`, but also in terms of the type of heap `HeapT` ("min heap" or "max heap").
In a "min heap", the `std::greater<T>{}` comparison function is used for the `heapify_up` and `heapify_down` methods, whereas in a "max heap", 
`std::less<T>{}` is used.

A number of utility factory functions are provided to easily create the type of Heap you want, namely:
- `make_min_heap<bool IsHeap = false, typename T>`: create a min binary heap starting from a vector of values.
- `make_max_heap<bool IsHeap = false, typename T>`: create a max binary heap starting from a vector of values.

Optionally, if you know that the vector you're feeding to `BinaryHeap` already respects the Heap property (i.e. `std::is_heap(your_vector)` evaluates to true), you can set the template argument `IsHeap` to true. This avoids the `O(n)` time `heapify()` step. This template argument is checked at compile time using an `if constexpr` statement.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/container/binary_heap.h>

int main() {
  std::vector<char> vec { 'b', 'c', 'f', 'a', 'e', 'd' };

  // initialize a min heap in O(n)
  auto min_heap(jkds::container::make_min_heap(std::move(vec)));

  while (!min_heap.empty()) {
    // get the minimum element in O(1)
    std::cout << "Min: " << min_heap.top() << "\n";
    
    // remove the minimum element in O(log_2 n)
    min_heap.pop();
  }

  // Output:
  // Min: a
  // Min: b
  // Min: c
  // Min: d
  // Min: e
  // Min: f

  std::cout << "\n\n";
  auto max_heap(jkds::container::make_max_heap<true, uint16_t>());

  // add a new element in O(log_2 n'), where n' is the current size of the heap
  max_heap.push(30);

  max_heap.push(15);
  max_heap.push(5000);
  max_heap.push(50);
  max_heap.push(599);

  while (!max_heap.empty()) {
    // get the maximum element in O(1)
    std::cout << "Max: " << max_heap.top() << "\n";

    // remove the minimum element in O(log_2 n)
    max_heap.pop();
  }

  // Output:
  // Max: 5000
  // Max: 599
  // Max: 50
  // Max: 30
  // Max: 15
}
```

### KHeap

The `KHeap<HeapT, T, IsHeap=false>` class (defined in [`k_heap.h`](`./include/jkds/container/k_heap.h`)) is a `K`-ary heap data structure,
i.e., a heap represented as a tree where the number of children at each level is at most `K`, for a given constant 2 < `K` <= 64.
It exposes the same API as `BinaryHeap`, but it requires `K` as an additional template parameter.
`KHeap`s can be more cache-friendly than `BinaryHeap`s.

A number of utility factory functions are provided to easily create the type of Heap you want, namely:
- `make_min_k_heap<std::size_t K, bool IsHeap = false, typename T>`: create a min `K`-ary heap starting from a vector of values.
- `make_max_k_heap<std::size_t K, bool IsHeap = false, typename T>`: create a max `K`-ary heap starting from a vector of values.

Optionally, if you know that the vector you're feeding to `KHeap` already respects the Heap property (i.e. `std::is_heap(your_vector)` evaluates to true), you can set the template argument `IsHeap` to true. This avoids the `O(n)` time `heapify()` step. This template argument is checked at compile time using an `if constexpr` statement.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/container/k_heap.h>

int main() {
  std::vector<char> vec { 'b', 'c', 'f', 'a', 'e', 'd' };
  constexpr std::size_t K1 = 3;

  // initialize a min heap in O(n)
  auto min_heap(jkds::container::make_min_k_heap<K1>(std::move(vec)));

  while (!min_heap.empty()) {
    // get the minimum element in O(1)
    std::cout << "Min: " << min_heap.top() << "\n";
    
    // remove the minimum element in O(log_4 n)
    min_heap.pop();
  }

  // Output:
  // Min: a
  // Min: b
  // Min: c
  // Min: d
  // Min: e
  // Min: f

  std::cout << "\n\n";
  constexpr std::size_t K2 = 4;
  auto max_heap(jkds::container::make_max_k_heap<K2, true, uint16_t>());

  // add a new element in O(log_4 n'), where n' is the current size of the heap
  max_heap.push(30);

  max_heap.push(15);
  max_heap.push(5000);
  max_heap.push(50);
  max_heap.push(599);

  while (!max_heap.empty()) {
    // get the maximum element in O(1)
    std::cout << "Max: " << max_heap.top() << "\n";

    // remove the minimum element in O(log_4 n)
    max_heap.pop();
  }

  // Output:
  // Max: 5000
  // Max: 599
  // Max: 50
  // Max: 30
  // Max: 15
}
```

### PriorityQueue

The `PriorityQueue<HeapT, HeapC, KeyT, ValueT, IsHeap>` class (defined in [`priority_queue.h`](`./include/jkds/container/priority_queue.h`))
models a priority queue based on a generic heap data structure.
The `PriorityQueue` privately extends either `BinaryHeap<HeapT, ValueT, IsHeap>` or `KHeap<K, HeapT, ValueT, IsHeap>` for some `K`.
A `SFINAE` check ensures that `PriorityQueue` can only extend a heap class compatible with the other template arguments received.

The main methods exposed by PriorityQueue are:

- `size()`: return the number of elements in the heap. Time complexity: `O(1)`.
- `empty()`: return true iff the heap is empty. Time complexity: `O(1)`.
- `push(const KeyT& key, const ValueT& element)`: add a new element to the heap and associates the given key to it. Time complexity: `O(log n)` amortized if using `BinaryHeap`, `O(k * log_k n)` amortized if using `KHeap`.
- `update_key(const KeyT& key, const ValueT& element)`: update the key of an existing element in the priority queue. Time: `O(log n)` amortized if using `BinaryHeap`, `O(k * log_k n)` amortized instead.
- `contains(const T& element)`: return true iff the given element is in the priority queue. Time: `O(1)` amortized.
- `top()`: return the top element. Time: `O(1)`.
- `pop()`: remove the top element. Time: `O(log n)` amortized if using `BinaryHeap`, `O(k * log_k n)` amortized instead.

**Note**: in order to keep `update_key`'s complexity logarithmic instead of linear, there's a quite important caveat: **arbitrary key updates are not allowed**. If you're using a priority queue based on a max heap, you can only perform an "increase key" operation, i.e., you should pass a `key` that is bigger than the previous one. On the other hand, if you're using a priority queue based on a min heap, you can only perform a "decrease key" operation. Arbitrary key updates may result in undefined behaviour.

**Note**: `PriorityQueue` is implemented using two `std::unordered_map<ValueT, *>` containers for fast retrieval of keys and element index given an element stored in the underlying heap. This implies that your values' types must have a `std::hash<ValueT>` implementation.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/container/priority_queue.h>

int main() {
  std::vector<std::size_t> keys1{ 5,   4,   1,   3,   6,   0,   2 };
  std::vector<char>        vec1{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
  auto min_pq1(jkds::container::make_min_priority_queue(keys1, vec1));

  while (!min_pq1.empty()) {
    const auto& [top_key, top_value] = min_pq1.top_key_value();
    std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
    min_pq1.pop();
  }

  // Output:
  // Min (key, value): (0, a)
  // Min (key, value): (1, n)
  // Min (key, value): (2, p)
  // Min (key, value): (3, h)
  // Min (key, value): (4, i)
  // Min (key, value): (5, m)
  // Min (key, value): (6, e)

  std::cout << std::endl << std::endl;

  // notice that keys already sorted
  std::vector<std::size_t> keys2{ 0,   2,   4,   6,   8,   10,  12 };
  std::vector<char>        vec2{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
  auto min_pq2(jkds::container::make_min_priority_queue<true>(keys2, vec2));

  while (!min_pq2.empty()) {
    const auto& [top_key, top_value] = min_pq2.top_key_value();
    std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
    min_pq2.pop();
  }

  // Output:
  // Min (key, value): (0, m)
  // Min (key, value): (2, i)
  // Min (key, value): (4, n)
  // Min (key, value): (6, h)
  // Min (key, value): (8, e)
  // Min (key, value): (10, a)
  // Min (key, value): (12, p)

  std::cout << std::endl << std::endl;
  auto min_pq2_update(jkds::container::make_min_priority_queue<true>(keys2, vec2));

  // Min Heap: keys can only decrease
  min_pq2_update.update_key(5, 'e'); // decrease the key associated to the value 'e' from 8 to 5
  min_pq2_update.update_key(1, 'p'); // decrease the key associated to the value 'p' from 12 to 1

  while (!min_pq2_update.empty()) {
    const auto& [top_key, top_value] = min_pq2_update.top_key_value();
    std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
    min_pq2_update.pop();
  }

  // Output:
  // Min (key, value): (0, m)
  // Min (key, value): (1, p)
  // Min (key, value): (2, i)
  // Min (key, value): (4, n)
  // Min (key, value): (5, e)
  // Min (key, value): (6, h)
  // Min (key, value): (10, a)
}
```

### DisjointSet

The `DisjointSet<T>` class (defined in [`disjoint_set.h`](`./include/jkds/container/disjoint_set.h`)) models a [disjoint set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) data structure (also known as union-find).
DisjointSet is optimized, as it implements the union-by-rank policy paired with path-splitting compression,
which results in almost constant time complexity for the main methods. 

We denote the [iterated logarithm](https://en.wikipedia.org/wiki/Iterated_logarithm) as `lg^*`.
For `n < 2^65536`, `(lg^* n) <= 5`.
The methods exposed by DisjointSet are:

- `add(const T& x)`: Add a new entry to the disjoint set, returning the index of the resulting node. Time complexity: `O(1)` amortized.
- `unite(const T& x, const T& y)`: Merge two dynamic sets which contain the x and y elements, respectively, into a new set
that is the union of the two sets.
x is assumed to be different than y. Time complexity: `O(lg^* n)` amortized.
- `are_connected(const T& x, const T& y)`: Return true if and only if the given two elements are in the same representative set. Time complexity: `O(lg^* n)` amortized.
- `get_sets()`: Snapshots the representative sets. Time complexity: `O(n + lg^* n)`.

**Note**: `DisjointSet<T>` is implemented using a `std::unordered_map<T, std::size_t>` container internally.
This implies that your values' types must have a `std::hash<T>` implementation.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/container/disjoint_set.h>

int main() {
  auto bool_to_string = [](const bool v) { return v ? "Yes" : "No"; };

  jkds::container::DisjointSet<char> ds{{'a', 'b', 'c', 'd', 'e'}};

  // initially every element is separated in its own representative set:
  // -> {{'a'}, {'b'}, {'c'}, {'d'}, {'e'}}

  std::cout << "Are 'a' and 'b' connected? " << bool_to_string(ds.are_connected('a', 'b')) << '\n';
  std::cout << "Are 'c' and 'd' connected? " << bool_to_string(ds.are_connected('c', 'd')) << '\n';

  // Output:
  // Are 'a' and 'b' connected? No
  // Are 'c' and 'd' connected? No

  ds.unite('a', 'b');
  ds.unite('c', 'd');

  // now 'a' is together with 'b', and 'c' is together with 'd':
  // -> {{'a', 'b'}, {'c', 'd'}, {'e'}}

  std::cout << "Are 'a' and 'b' connected? " << bool_to_string(ds.are_connected('a', 'b')) << '\n';
  std::cout << "Are 'c' and 'd' connected? " << bool_to_string(ds.are_connected('c', 'd')) << '\n';

  // Output:
  // Are 'a' and 'b' connected? Yes
  // Are 'c' and 'd' connected? Yes

  ds.add('f');

  // now 'f' is a new entry of the disjoint set:
  // -> {{'a', 'b'}, {'c', 'd'}, {'e'}, {'f'}}
}
```

### SparseByteSet

The `SparseByteSet` class (defined in [`sparse_byte_set.h`](`./include/jkds/container/sparse_byte_set.h`)) represents
an append-only set of bytes. Its capacity is fixed at 256, and it can be used as a replacement of `std::bitset<256>`.
Adding a `uint8_t` element and checking whether an `uint8_t` element exist can both be performed in constant time.

#### Example usage

```c++
#include <iostream>
#include <jkds/container/sparse_byte_set.h>

int main() {
  // immediately allocate 256 bytes of memory on the stack
  jkds::container::SparseByteSet s;
  
  // add an element in O(1)
  s.add(63);

  s.add(127)
  s.add(255);

  // check whether an element appears in the set in O(1)
  std::cout << "127 " << s.contains(127) ? "appears" : "doesn't appear" << "\n";

  // Output:
  // 127 appears

  s.reset();
  std::cout << "127 " << s.contains(127) ? "appears" : "doesn't appear" << "\n";

  // Output:
  // 127 doesn't appear
}
```

## jkds::functional

The functional programming abstract utilities are defined in [`./include/jkds/functional`](`./include/jkds/functional`).
The goal of the `jkds::functional` namespace is to allow the C++ user to adopt patterns that are common in pure functional languages like Haskell and Scala,
without compromising the idiomaticity of the C++ code.

### fmap

The `fmap` function (defined in [`fmap.h`](`./include/jkds/functional/fmap.h`)) is a higher-order function that applies a function to each element of an iterable container and returns a vector of the results.
It's conceptually equivalent to Haskell's `fmap :: (a -> b) -> f a -> f b` function (in which case, the function `f` is called a ["functor"](https://hackage.haskell.org/package/base-4.16.0.0/docs/Data-Functor.html)).

#### Example usage

```c++
#include <iostream>
#include <string>
#include <vector>
#include <jkds/functional/fmap.h>

int main() {
  std::vector<int> numbers{1, 2, 3, 4, 5};
  std::vector<std::string> strings{"1", "2", "3", "4", "5"};

  std::vector<string> out_strings = jkds::functional::fmap([](int n) { return to_string(n); }, numbers);
  for (auto&& s : out_strings) {
    std::cout << "s: " << s << ";";
  }

  // Output:
  // 1; 2; 3; 4; 5;

  std::cout << "\n\n";
  std::vector<string> out_strings_partial = fmap([](int n) { return to_string(n); }, numbers.cbegin(), numbers.cbegin() + 3);
  for (auto&& s : out_strings_partial) {
    std::cout << "s: " << s << ";";
  }

  // Output:
  // 1; 2; 3;
}
```

### zip

The `zip` function (defined in [`zip.h`](`./include/jkds/functional/zip.h`)) is a utility that allows iterating over 2 or more containers at the same time. The number of iterations is bounded by the minimum size of the given iterables.
It's conceptually equivalent to Python's [`zip(*iterables)`](https://docs.python.org/3/library/functions.html#zip) function.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/functional/zip.h>

int main() {
  std::vector<uint8_t> first{1, 2, 3, 4, 5};
  std::vector<char> second{'a', 'b', 'c'};

  for (auto&& [a, b] : jkds::functional::zip(first, second)) {
    std::cout << "(" << a << ", " << b << ")\n";
  }

  // Output:
  // (1, a)
  // (2, b)
  // (3, c)
}
```

## jkds::util

The general purpose utilities are defined in [`./include/jkds/util`](`./include/jkds/util`).
They are mainly used as auxiliary functions for `jkds::container`, but they may also be useful as standalone utilities.

### range

The `range` function (defined in [`range.h`](`./include/jkds/util/range.h`)) generates a sequential range of values of a given size.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/util/range.h>

int main() {
  auto vec(range<size_t>(4, 100));

  for (auto&& x : vec) {
    std::cout << x ", ";
  }

  // Output:
  // 100, 101, 102, 103
}
```

#### resize

The `resize` function (defined in [`resize.h`](`./include/jkds/util/resize.h`)) resizes any STL-compliant
container without requiring it's value type to be default-constructible.

#### Example usage

```c++
#include <iostream>
#include <vector>
#include <jkds/util/resize.h>

struct Pod {
  char value;

  Pod() = delete;
  Pod(char value) : value(value) {
  }

  Pod(const Pod& other) = default;
  auto operator<=>(const Pod& rhs) const = default;
};

int main() {
  std::vector<Pod> vec{Pod('a'), Pod('b'), Pod('c'), Pod('d')};

  // This would not compile, as the compiler wouldn't find the proper overload for std::construct_at:
  // vec.resize(2);

  // This compiles
  jkds::util::resize(vec, 2);

  for (auto&& x : vec) {
    std::cout << x ", ";
  }

  // Output:
  // a, b
}
```

### shift_to_value

The `shift_to_value` function (defined in [`shift_to_value.h`](`./include/jkds/util/shift_to_value.h`))
rotates a given container based on the first occurrence of the given value.

#### Example usage

```c++
#include <cstdint>
#include <iostream>
#include <vector>
#include <jkds/util/shift_to_value.h>

int main() {
  std::vector<uint8_t> vec{0, 10, 10, 10, 20};
  jkds::util::shift_to_value(vec, 10);

  for (auto&& x : vec) {
    std::cout << x ", ";
  }

  // Output:
  // 10, 10, 10, 20, 0
}
```

# Authors

- Alberto Schiabel: [Github](https://github.com/jkomyno), [Twitter](https://twitter.com/jkomyno)

# Contributing

Contributions and issues are welcome!
Please, before proposing a pull request, open an issue first.

# Show your support

Give a ⭐️to this repository if `jkds` helped or inspired you! In the future, I might consider offering premium support to Github Sponsors.

# Related Repositories

The following are some related, deprecated repositories written by Alberto Schiabel that inspired the creation of `jkds`.

- [`priority-queue`](https://github.com/jkomyno/priority-queue)
- [`disjoint-set`](https://github.com/jkomyno/disjoint-set)
