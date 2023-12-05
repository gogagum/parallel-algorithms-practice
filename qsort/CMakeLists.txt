cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(sort_tests sort_test.cpp)
target_include_directories(sort_tests PUBLIC src)
target_link_libraries(sort_tests PUBLIC gtest_main tbb)

add_executable(sort_benchmarks sort_benchmarks.cpp)
target_include_directories(sort_benchmarks PUBLIC src)
target_link_libraries(sort_benchmarks PUBLIC benchmark_main tbb)

include(GoogleTest)
gtest_discover_tests(sort_tests)
