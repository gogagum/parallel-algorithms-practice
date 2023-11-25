#ifndef PARALLEL_QUICKSORT_HPP
#define PARALLEL_QUICKSORT_HPP

#include "impl/cmp_tools.hpp"
#include <algorithm>
#include <execution>
#include <iterator>
#include <vector>

template <class Iterator>
void parallel_quicksort(Iterator begin, Iterator end) {
  using T = typename std::iterator_traits<Iterator>::value_type;
  const auto size = end - begin;
  if (size <= 1) {
    return;
  }
  T supportElement = *begin;
  auto lower = std::vector<T>{};
  lower.reserve(size);
  std::copy_if(std::execution::par, begin + 1, end, std::back_inserter(lower),
               impl::LessThan<T>{supportElement});
  parallel_quicksort(lower.begin(), lower.end());
  auto higher = std::vector<T>{};
  higher.reserve(size);
  std::copy_if(std::execution::par, begin + 1, end, std::back_inserter(higher),
               impl::NotLessThan<T>{supportElement});
  parallel_quicksort(higher.begin(), higher.end());
  auto outIter =
      std::copy(std::execution::par, lower.begin(), lower.end(), begin);
  *outIter = std::move(supportElement);
  ++outIter;
  std::copy(std::execution::par, higher.begin(), higher.end(), outIter);
}

#endif // PARALLEL_QUICKSORT_HPP
