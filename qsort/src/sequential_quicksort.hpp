#ifndef SEQUENTIAL_QUICKSORT_HPP
#define SEQUENTIAL_QUICKSORT_HPP

#include "impl/cmp_tools.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

template <class Iterator>
void sequential_quicksort(Iterator begin, Iterator end) {
  using T = typename std::iterator_traits<Iterator>::value_type;
  const auto size = end - begin;
  if (size <= 1) {
    return;
  }
  T supportElement = *begin;
  auto lower = std::vector<T>{};
  lower.reserve(size);
  std::copy_if(begin + 1, end, std::back_inserter(lower),
               impl::LessThan<T>{supportElement});
  sequential_quicksort(lower.begin(), lower.end());
  auto higher = std::vector<T>{};
  higher.reserve(size);
  std::copy_if(begin + 1, end, std::back_inserter(higher),
               impl::NotLessThan<T>{supportElement});
  sequential_quicksort(higher.begin(), higher.end());
  auto outIter = std::copy(lower.begin(), lower.end(), begin);
  *outIter = std::move(supportElement);
  ++outIter;
  std::copy(higher.begin(), higher.end(), outIter);
}

#endif // SEQUENTIAL_QUICKSORT_HPP
