#ifndef SORT_TEST_HPP
#define SORT_TEST_HPP

#include "parallel_forkjoin_quicksort.hpp"
#include "parallel_naive_quicksort.hpp"
#include "parallel_selective_quicksort.hpp"
#include "sequential_quicksort.hpp"
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

enum AlgoType {
  SEQUENTIAL = 0,
  PARALLEL_NAIVE = 1,
  PARALLEL_SELECTIVE = 2,
  PARALLEL_FORKJOIN = 3
};

using SortTestParam =
    std::tuple<std::string /*description*/, AlgoType /*parallel*/,
               std::vector<int> /*to sort*/>;

class SortTest : public testing::TestWithParam<SortTestParam> {};

TEST_P(SortTest, CompareWithStd) {
  const auto &[_, algoType, toSort] = SortTest::GetParam();

  auto toSortWithTested = std::vector<int>(toSort);
  auto toSortWithStd = std::vector<int>(toSort);

  switch (algoType) {
  case SEQUENTIAL:
    sequential_quicksort(toSortWithTested.begin(), toSortWithTested.end());
    break;
  case PARALLEL_NAIVE:
    parallel_naive_quicksort(toSortWithTested.begin(), toSortWithTested.end());
    break;
  case PARALLEL_SELECTIVE:
    parallel_selective_quicksort(toSortWithTested.begin(),
                                 toSortWithTested.end(),
                                 std::max<std::size_t>(1, toSort.size() / 5));
    break;
  case PARALLEL_FORKJOIN:
    parallel_forkjoin_quicksort(toSortWithTested.begin(),
                                toSortWithTested.end(), 4);
    break;
  default:
    assert(false && "Test for algo is not implemented.");
    break;
  }

  std::sort(toSortWithStd.begin(), toSortWithStd.end());

  EXPECT_EQ(toSortWithTested, toSortWithStd);
}

const static auto testCases =
    std::vector<SortTestParam>{{"EmptyVec", SEQUENTIAL, {}},
                               {"EmptyVec", PARALLEL_NAIVE, {}},
                               {"EmptyVec", PARALLEL_SELECTIVE, {}},
                               {"EmptyVec", PARALLEL_FORKJOIN, {}},
                               {"SmallVec", SEQUENTIAL, {3, 4, 1, 2}},
                               {"SmallVec", PARALLEL_NAIVE, {3, 4, 1, 2}},
                               {"SmallVec", PARALLEL_SELECTIVE, {3, 4, 1, 2}},
                               {"SmallVec", PARALLEL_FORKJOIN, {3, 4, 1, 2}},
                               {"Sorted", SEQUENTIAL, {1, 2, 3, 4, 5}},
                               {"Sorted", PARALLEL_NAIVE, {1, 2, 3, 4, 5}},
                               {"Sorted", PARALLEL_SELECTIVE, {1, 2, 3, 4, 5}},
                               {"Sorted", PARALLEL_FORKJOIN, {1, 2, 3, 4, 5}}};

const static auto generateName =
    [](const testing::TestParamInfo<SortTestParam> &info) -> std::string {
  const auto &descr = std::get<0>(info.param);
  const auto &parallel = std::get<1>(info.param);
  std::string suffix;
  switch (parallel) {
  case SEQUENTIAL:
    suffix = "Sequential";
    break;
  case PARALLEL_NAIVE:
    suffix = "ParallelNaive";
    break;
  case PARALLEL_SELECTIVE:
    suffix = "ParallelSelective";
    break;
  case PARALLEL_FORKJOIN:
    suffix = "ParallelForkjoin";
    break;
  default:
    assert(false && "Algo test is not implemented.");
    break;
  }
  return descr + suffix;
};

INSTANTIATE_TEST_SUITE_P(SortUnitTests, SortTest, testing::ValuesIn(testCases),
                         generateName);

SortTestParam generate_test_case(std::size_t size, AlgoType parallel,
                                 std::uint32_t seed = 0) {
  auto toSort = std::vector<int>{};
  auto gen = std::mt19937(seed);
  std::generate_n(std::back_inserter(toSort), size, gen);
  return {"GeneratedTestOfSize" + std::to_string(size), parallel,
          std::move(toSort)};
}

const static auto generatedTests =
    std::vector<SortTestParam>{generate_test_case(10, SEQUENTIAL),
                               generate_test_case(10, PARALLEL_NAIVE),
                               generate_test_case(10, PARALLEL_SELECTIVE),
                               generate_test_case(10, PARALLEL_FORKJOIN),
                               generate_test_case(100, SEQUENTIAL),
                               generate_test_case(100, PARALLEL_NAIVE),
                               generate_test_case(100, PARALLEL_SELECTIVE),
                               generate_test_case(100, PARALLEL_FORKJOIN),
                               generate_test_case(1000, SEQUENTIAL),
                               generate_test_case(1000, PARALLEL_NAIVE),
                               generate_test_case(1000, PARALLEL_SELECTIVE),
                               generate_test_case(1000, PARALLEL_FORKJOIN)};

INSTANTIATE_TEST_SUITE_P(SortGeneratedTests, SortTest,
                         testing::ValuesIn(generatedTests), generateName);

#endif
