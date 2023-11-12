cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(sequential_sort_tests sequential_sort_tests.cpp)
target_include_directories(sequential_sort_tests PUBLIC src)
target_link_libraries(sequential_sort_tests PUBLIC gtest_main)

add_executable(parallel_sort_tests parallel_sort_tests.cpp)
target_include_directories(parallel_sort_tests PUBLIC src)
target_link_libraries(parallel_sort_tests PUBLIC gtest_main tbb)

include(GoogleTest)
gtest_discover_tests(sequential_sort_tests)
gtest_discover_tests(parallel_sort_tests)
