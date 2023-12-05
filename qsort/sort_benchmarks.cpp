#include "parallel_forkjoin_quicksort.hpp"
#include "parallel_naive_quicksort.hpp"
#include "parallel_selective_quicksort.hpp"
#include "sequential_quicksort.hpp"
#include <benchmark/benchmark.h>
#include <oneapi/tbb/global_control.h>

#include <random>

static void BM_benchmark_sequential_quicksort(benchmark::State &state) {
  const auto testSize = static_cast<std::size_t>(state.range(0));

  for ([[maybe_unused]] const auto st : state) {
    state.PauseTiming();
    auto toSort = std::vector<int>(testSize);
    auto gen = std::mt19937();
    std::generate_n(toSort.begin(), testSize, gen);
    state.ResumeTiming();
    sequential_quicksort(toSort.begin(), toSort.end());
  }
}

static void BM_benchmark_parallel_naive_quicksort(benchmark::State &state) {
  oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

  const auto testSize = static_cast<std::size_t>(state.range(0));

  for ([[maybe_unused]] const auto st : state) {
    state.PauseTiming();
    auto toSort = std::vector<int>(testSize);
    auto gen = std::mt19937();
    std::generate_n(toSort.begin(), testSize, gen);
    state.ResumeTiming();
    parallel_naive_quicksort(toSort.begin(), toSort.end());
  }
}

static void
BM_benchmark_parallel_selective_quicksort_1000(benchmark::State &state) {
  oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

  const auto testSize = static_cast<std::size_t>(state.range(0));

  for ([[maybe_unused]] const auto st : state) {
    state.PauseTiming();
    auto toSort = std::vector<int>(testSize);
    auto gen = std::mt19937();
    std::generate_n(toSort.begin(), testSize, gen);
    state.ResumeTiming();
    parallel_selective_quicksort(toSort.begin(), toSort.end(), 1000);
  }
}

static void
BM_benchmark_parallel_selective_quicksort_100000(benchmark::State &state) {
  oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

  const auto testSize = static_cast<std::size_t>(state.range(0));

  for ([[maybe_unused]] const auto st : state) {
    state.PauseTiming();
    auto toSort = std::vector<int>(testSize);
    auto gen = std::mt19937();
    std::generate_n(toSort.begin(), testSize, gen);
    state.ResumeTiming();
    parallel_selective_quicksort(toSort.begin(), toSort.end(), 100000);
  }
}

static void BM_benchmark_parallel_forkjoin_quicksort(benchmark::State &state) {
  const auto testSize = static_cast<std::size_t>(state.range(0));

  for ([[maybe_unused]] const auto st : state) {
    state.PauseTiming();
    auto toSort = std::vector<int>(testSize);
    auto gen = std::mt19937();
    std::generate_n(toSort.begin(), testSize, gen);
    state.ResumeTiming();
    parallel_forkjoin_quicksort(toSort.begin(), toSort.end(), 4);
  }
}

constexpr static auto testSize = std::size_t{100000000};

BENCHMARK(BM_benchmark_sequential_quicksort)->Arg(testSize)->Iterations(5);

BENCHMARK(BM_benchmark_parallel_naive_quicksort)->Arg(testSize)->Iterations(5);
BENCHMARK(BM_benchmark_parallel_selective_quicksort_1000)
    ->Arg(testSize)
    ->Iterations(5);
BENCHMARK(BM_benchmark_parallel_selective_quicksort_100000)
    ->Arg(testSize)
    ->Iterations(5);
BENCHMARK(BM_benchmark_parallel_forkjoin_quicksort)
    ->Arg(testSize)
    ->Iterations(5);
