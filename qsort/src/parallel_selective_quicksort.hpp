#ifndef PARALLEL_SELECTIVE_QUICKSORT_HPP
#define PARALLEL_SELECTIVE_QUICKSORT_HPP

#include "impl/cmp_tools.hpp"
#include "sequential_quicksort.hpp"
#include <algorithm>
#include <execution>
#include <iterator>
#include <vector>

template <class Iterator>
void parallel_selective_quicksort(Iterator begin, Iterator end,
                                  const std::size_t minParallelSize) {
  using T = typename std::iterator_traits<Iterator>::value_type;
  const auto size = end - begin;
  if (size <= 1) {
    return;
  }
  T supportElement = *begin;
  auto lower = std::vector<T>{};
  lower.reserve(size);
  std::copy_if(std::execution::par_unseq, begin + 1, end,
               std::back_inserter(lower), impl::LessThan<T>{supportElement});
  auto higher = std::vector<T>{};
  higher.reserve(size);
  std::copy_if(std::execution::par_unseq, begin + 1, end,
               std::back_inserter(higher),
               impl::NotLessThan<T>{supportElement});
  auto outIter = begin;
  if (lower.size() >= minParallelSize) {
    parallel_selective_quicksort(lower.begin(), lower.end(), minParallelSize);
    outIter = std::copy(std::execution::par_unseq, lower.begin(), lower.end(),
                        outIter);
  } else {
    sequential_quicksort(lower.begin(), lower.end());
    outIter = std::copy(lower.begin(), lower.end(), outIter);
  }
  *outIter = std::move(supportElement);
  ++outIter;
  if (higher.size() >= minParallelSize) {
    parallel_selective_quicksort(higher.begin(), higher.end(), minParallelSize);
    std::copy(std::execution::par_unseq, higher.begin(), higher.end(), outIter);
  } else {
    sequential_quicksort(higher.begin(), higher.end());
    std::copy(higher.begin(), higher.end(), outIter);
  }
}

#endif // PARALLEL_SELECTIVE_QUICKSORT_HPP
