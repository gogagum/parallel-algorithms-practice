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
)

target_include_directories(graph_tests PUBLIC src)
target_link_libraries(graph_tests PUBLIC gtest_main tbb)

target_include_directories(sequential_bfs_tests PUBLIC src)
target_link_libraries(sequential_bfs_tests PUBLIC gtest_main tbb)

include(GoogleTest)
gtest_discover_tests(graph_tests)
gtest_discover_tests(sequential_bfs_tests)
