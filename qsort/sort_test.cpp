#ifndef SORT_TEST_HPP
#define SORT_TEST_HPP

#include "parallel_quicksort.hpp"
#include "sequential_quicksort.hpp"
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

using SortTestParam = std::tuple<std::string /*description*/, bool /*parallel*/,
                                 std::vector<int> /*to sort*/>;

class SortTest : public testing::TestWithParam<SortTestParam> {};

TEST_P(SortTest, CompareWithStd) {
  const auto &[_, parallel, toSort] = SortTest::GetParam();

  auto toSortWithTested = std::vector<int>(toSort);
  auto toSortWithStd = std::vector<int>(toSort);

  if (parallel) {
    parallel_quicksort(toSortWithTested.begin(), toSortWithTested.end());
  } else {
    sequential_quicksort(toSortWithTested.begin(), toSortWithTested.end());
  }

  std::sort(toSortWithStd.begin(), toSortWithStd.end());

  EXPECT_EQ(toSortWithTested, toSortWithStd);
}

const static auto testCases =
    std::vector<SortTestParam>{{"EmptyVec", false, {}},
                               {"EmptyVec", true, {}},
                               {"SmallVec", false, {3, 4, 1, 2}},
                               {"SmallVec", true, {3, 4, 1, 2}},
                               {"Sorted", false, {1, 2, 3, 4, 5}},
                               {"Sorted", true, {1, 2, 3, 4, 5}}};

const static auto generateName =
    [](const testing::TestParamInfo<SortTestParam> &info) -> std::string {
  const auto &descr = std::get<0>(info.param);
  const auto &parallel = std::get<1>(info.param);
  return descr + (parallel ? "Parallel" : "Sequential");
};

INSTANTIATE_TEST_SUITE_P(SortUnitTests, SortTest, testing::ValuesIn(testCases),
                         generateName);

SortTestParam generate_test_case(std::size_t size, bool parallel,
                                 std::uint32_t seed = 0) {
  auto toSort = std::vector<int>{};
  auto gen = std::mt19937(seed);
  std::generate_n(std::back_inserter(toSort), size, gen);
  return {"GeneratedTestOfSize" + std::to_string(size), parallel,
          std::move(toSort)};
}

const static auto generatedTests = std::vector<SortTestParam>{
    generate_test_case(10, false),   generate_test_case(10, true),
    generate_test_case(100, false),  generate_test_case(100, true),
    generate_test_case(1000, false), generate_test_case(1000, true)};

INSTANTIATE_TEST_SUITE_P(SortGeneratedTests, SortTest,
                         testing::ValuesIn(generatedTests), generateName);

#endif
