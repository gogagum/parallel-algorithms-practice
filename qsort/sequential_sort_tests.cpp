#include "sequential_quicksort.hpp"
#include <gtest/gtest.h>

#include <vector>

TEST(SequentialQuicksort, SortEmpty) {
  auto sorted = std::vector<int>{};

  sequential_quick_sort(sorted.begin(), sorted.end());

  EXPECT_TRUE(sorted.empty());
}

TEST(SequentialQuicksort, SortTwoSortedElements) {
  auto sorted = std::vector<int>{1, 2};
  const auto expected = std::vector<int>{1, 2};

  sequential_quick_sort(sorted.begin(), sorted.end());

  EXPECT_EQ(sorted, expected);
}

TEST(SequentialQuicksort, SortFiveElements) {
  auto sorted = std::vector<int>{5, 6, 4, 2, 2};
  const auto expected = std::vector<int>{2, 2, 4, 5, 6};

  sequential_quick_sort(sorted.begin(), sorted.end());

  EXPECT_EQ(sorted, expected);
}