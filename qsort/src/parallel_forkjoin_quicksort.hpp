#ifndef PARALLEL_FORKJOIN_QUICKSORT_HPP
#define PARALLEL_FORKJOIN_QUICKSORT_HPP

#include "impl/cmp_tools.hpp"
#include "sequential_quicksort.hpp"
#include <algorithm>
#include <iterator>
#include <oneapi/tbb/parallel_invoke.h>
#include <vector>

template <class Iterator>
void parallel_forkjoin_quicksort(Iterator begin, Iterator end,
                                 const std::size_t procLimit) {
  if (1 == procLimit) {
    sequential_quicksort(begin, end);
    return;
  }

  using T = typename std::iterator_traits<Iterator>::value_type;
  const auto size = end - begin;
  if (size <= 1) {
    return;
  }
  T supportElement = *begin;
  auto lower = std::vector<T>{};
  lower.reserve(size);
  auto higher = std::vector<T>{};
  higher.reserve(size);

  const auto firstLimit = (procLimit + 1) / 2;
  const auto secondLimit = procLimit - firstLimit;

  const auto takeAndSortLower = [&lower, begin, end, firstLimit,
                                 &supportElement]() {
    std::copy_if(begin + 1, end, std::back_inserter(lower),
                 impl::LessThan<T>{supportElement});
    parallel_forkjoin_quicksort(lower.begin(), lower.end(), firstLimit);
  };

  const auto takeAndSortHigher = [&higher, begin, end, secondLimit,
                                  &supportElement]() {
    std::copy_if(begin + 1, end, std::back_inserter(higher),
                 impl::NotLessThan<T>{supportElement});
    parallel_forkjoin_quicksort(higher.begin(), higher.end(), secondLimit);
  };

  tbb::parallel_invoke(takeAndSortLower, takeAndSortHigher);

  auto outIter = std::copy(lower.begin(), lower.end(), begin);
  *outIter = std::move(supportElement);
  ++outIter;
  std::copy(higher.begin(), higher.end(), outIter);
}

#endif // PARALLEL_FORKJOIN_QUICKSORT_HPP
