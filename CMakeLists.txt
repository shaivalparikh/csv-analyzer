cmake_minimum_required(VERSION 3.20)
project(csv_analyzer CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(csvlib src/csv.cpp src/stats.cpp)
target_include_directories(csvlib PUBLIC include)
target_compile_options(csvlib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(analyze app/main.cpp)
target_link_libraries(analyze PRIVATE csvlib)
target_compile_options(analyze PRIVATE -Wall -Wextra -Wpedantic)