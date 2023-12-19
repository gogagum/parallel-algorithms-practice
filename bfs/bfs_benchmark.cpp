#include "cube_experimental_graph_filler.hpp"
#include <parallel_distance_counting_bfs.hpp>
#include <sequential_distance_counting_bfs.hpp>
#include <benchmark/benchmark.h>
#include <oneapi/tbb/global_control.h>

static void BM_benchmark_sequential_bfs(benchmark::State& state) {
    for ([[maybe_unused]] const auto st: state) {
        state.PauseTiming();
        auto graph = CubeExperimentalGraphFiller(500).generate();
        state.ResumeTiming();
        sequential_distance_counting_bfs(graph);
    }
}

static void BM_benchmark_parallel_bfs(benchmark::State& state) {
    oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

    for ([[maybe_unused]] const auto st: state) {
        state.PauseTiming();
        auto graph = CubeExperimentalGraphFiller(500).generate();
        state.ResumeTiming();
        parallel_distance_counting_bfs(graph);
    }
}

BENCHMARK(BM_benchmark_sequential_bfs)->Iterations(5);
BENCHMARK(BM_benchmark_parallel_bfs)->Iterations(5);
