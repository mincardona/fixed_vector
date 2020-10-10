cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

# main project
project(mji-fixed-array VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

# header library
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(fixed-array INTERFACE)
set(lib_include_dir "include/")
set(lib_header "${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}/mji/fixed_vector.hpp")
target_include_directories(fixed-array INTERFACE "${lib_include_dir}")
target_sources(fixed-array INTERFACE "${lib_header}")

# tests
add_executable(${PROJECT_NAME}-tests "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")
target_compile_options(${PROJECT_NAME}-tests PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>)
target_link_libraries(${PROJECT_NAME}-tests PRIVATE fixed-array)
add_test(NAME mji::fixed_array::tests
         CONFIGURATIONS DEBUG
         COMMAND ${PROJECT_NAME}-tests)
