project(HaffmanArchiever)
cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
find_package(GTest REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
