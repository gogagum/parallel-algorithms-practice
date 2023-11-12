#ifndef SEQUENTIAL_QUICKSORT_HPP
#define SEQUENTIAL_QUICKSORT_HPP

#include <algorithm>
#include <iterator>
#include <vector>
#include <cassert>

template <class Iterator> void sequential_quick_sort(Iterator begin, Iterator end) {
  using T = typename std::iterator_traits<Iterator>::value_type;
  const auto size = end - begin;
  if (size <= 1) {
    return;
  }
  T supportElement = *begin;
  auto lower = std::vector<T>{};
  lower.reserve(size);
  std::copy_if(
      begin + 1, end, std::back_inserter(lower),
      [&supportElement](const T &element) { return element < supportElement; });
  sequential_quick_sort(lower.begin(), lower.end());
  auto higher = std::vector<T>{};
  higher.reserve(size);
  std::copy_if(begin + 1, end, std::back_inserter(higher),
               [&supportElement](const T &element) {
                 return element >= supportElement;
               });
  assert(higher.size() + lower.size() + 1 == size);
  sequential_quick_sort(higher.begin(), higher.end());
  auto outIter = std::copy(lower.begin(), lower.end(), begin);
  *outIter = std::move(supportElement);
  ++outIter;
  std::copy(higher.begin(), higher.end(), outIter);
}

#endif // SEQUENTIAL_QUICKSORT_HPP
