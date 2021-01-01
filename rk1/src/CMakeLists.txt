cmake_minimum_required(VERSION 3.0)

project(rk1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O0")

# find_package(benchmark REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

add_executable(app          main.cpp)
# add_executable(benchmark    benchmark.cpp)

target_link_libraries(app pthread)
# target_link_libraries(benchmark benchmark::benchmark)
