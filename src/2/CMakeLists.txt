cmake_minimum_required(VERSION 3.0)

project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O0")

add_compile_options(
    # -Werror
    # -Wall
    # -Wextra
    # -Wpedantic
)


find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})


# set(MAT matrix/base_matrix.cpp)

add_executable(app          main.cpp        ${MAT})
add_executable(test         test.cpp        ${MAT})
add_executable(benchmark    benchmark.cpp   ${MAT})


target_link_libraries(test ${GTEST_LIBRARIES} pthread)
target_link_libraries(benchmark benchmark::benchmark)


# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake