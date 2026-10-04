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

set(SAMPLE ${CMAKE_SOURCE_DIR}/tests/data/sensors.csv)

add_test(NAME cli_default_report COMMAND analyze ${SAMPLE})
set_tests_properties(cli_default_report PROPERTIES
  PASS_REGULAR_EXPRESSION "Loaded 40 rows, 4 columns")

add_test(NAME cli_custom_columns COMMAND analyze ${SAMPLE} sensor_id humidity)
set_tests_properties(cli_custom_columns PROPERTIES
  PASS_REGULAR_EXPRESSION "mean humidity")

add_test(NAME cli_no_args_fails COMMAND analyze)
set_tests_properties(cli_no_args_fails PROPERTIES WILL_FAIL TRUE)

add_test(NAME cli_unknown_column_fails COMMAND analyze ${SAMPLE} sensor_id nope)
set_tests_properties(cli_unknown_column_fails PROPERTIES WILL_FAIL TRUE)

add_test(NAME cli_missing_file_fails COMMAND analyze does_not_exist.csv)
set_tests_properties(cli_missing_file_fails PROPERTIES WILL_FAIL TRUE)
