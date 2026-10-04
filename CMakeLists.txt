cmake_minimum_required(VERSION 3.24)
project(csv_analyzer CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(csvlib src/csv.cpp src/stats.cpp)
target_include_directories(csvlib PUBLIC include)
target_compile_options(csvlib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(analyze app/main.cpp)
target_link_libraries(analyze PRIVATE csvlib)
target_compile_options(analyze PRIVATE -Wall -Wextra -Wpedantic)

include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(unit_tests tests/test_csv.cpp tests/test_stats.cpp)
target_link_libraries(unit_tests PRIVATE csvlib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)