#include "cube_experimental_graph_filler.hpp"
#include <parallel_distance_counting_bfs.hpp>
#include <sequential_distance_counting_bfs.hpp>
#include <benchmark/benchmark.h>
#include <oneapi/tbb/global_control.h>

static void BM_benchmark_sequential_bfs(benchmark::State& state) {
    auto graph = CubeExperimentalGraphFiller(300).generate();
    
    for ([[maybe_unused]] const auto st: state) {
        sequential_distance_counting_bfs(graph);
        state.PauseTiming();
        graph.resetDistancesToDefault();
        state.ResumeTiming();
    }
}

static void BM_benchmark_parallel_bfs(benchmark::State& state) {
    oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

    auto graph = CubeExperimentalGraphFiller(300).generate();

    for ([[maybe_unused]] const auto st: state) {
        parallel_distance_counting_bfs(graph);
        state.PauseTiming();
        graph.resetDistancesToDefault();
        state.ResumeTiming();
    }
}

template <std::size_t blockSize>
static void BM_benchmark_parallel_bfs_blocked(benchmark::State& state) {
    oneapi::tbb::global_control global_limit(
      oneapi::tbb::global_control::max_allowed_parallelism, 4);

    auto graph = CubeExperimentalGraphFiller(300).generate();

    for ([[maybe_unused]] const auto st: state) {
        parallel_distance_counting_bfs_blocked<blockSize>(graph);
        state.PauseTiming();
        graph.resetDistancesToDefault();
        state.ResumeTiming();
    }
}

BENCHMARK(BM_benchmark_sequential_bfs)->Iterations(5);
BENCHMARK(BM_benchmark_parallel_bfs)->Iterations(5);
BENCHMARK(BM_benchmark_parallel_bfs_blocked<1024>)->Iterations(5);
BENCHMARK(BM_benchmark_parallel_bfs_blocked<2048>)->Iterations(5);

