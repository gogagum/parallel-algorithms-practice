cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)

FetchContent_MakeAvailable(googlebenchmark)

enable_testing()

add_executable(
    graph_tests
    graph_tests.cpp
    src/experimental_graph.cpp
    src/node.cpp
    src/cube_graph_coordinates_converter.cpp
    src/cube_experimental_graph_filler.cpp
)

add_executable(
    sequential_bfs_tests
    sequential_bfs_tests.cpp
    src/experimental_graph.cpp
    src/node.cpp
    src/cube_graph_coordinates_converter.cpp
    src/cube_experimental_graph_filler.cpp
    src/sequential_distance_counting_bfs.cpp
)

add_executable(
    parallel_bfs_tests
    parallel_bfs_tests.cpp
    src/experimental_graph.cpp
    src/node.cpp
    src/cube_graph_coordinates_converter.cpp
    src/cube_experimental_graph_filler.cpp
    src/parallel_distance_counting_bfs.cpp
)

add_executable(
    bfs_benchmark
    bfs_benchmark.cpp
    src/experimental_graph.cpp
    src/node.cpp
    src/parallel_distance_counting_bfs.cpp
    src/cube_graph_coordinates_converter.cpp
    src/cube_experimental_graph_filler.cpp
    src/sequential_distance_counting_bfs.cpp
)

target_include_directories(graph_tests PUBLIC src)
target_link_libraries(graph_tests PUBLIC gtest_main tbb)

target_include_directories(sequential_bfs_tests PUBLIC src)
target_link_libraries(sequential_bfs_tests PUBLIC gtest_main tbb)

target_include_directories(parallel_bfs_tests PUBLIC src)
target_link_libraries(parallel_bfs_tests PUBLIC gtest_main tbb)

target_include_directories(bfs_benchmark PUBLIC src)
target_link_libraries(bfs_benchmark PUBLIC benchmark_main tbb)

include(GoogleTest)
gtest_discover_tests(graph_tests)
gtest_discover_tests(sequential_bfs_tests)
gtest_discover_tests(parallel_bfs_tests)
