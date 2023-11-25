cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(sort_tests sort_test.cpp)
target_include_directories(sort_tests PUBLIC src)
target_link_libraries(sort_tests PUBLIC gtest_main tbb)

include(GoogleTest)
gtest_discover_tests(sort_tests)
