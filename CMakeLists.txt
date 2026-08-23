cmake_minimum_required(VERSION 3.20)

project(my_c_cpp_playground VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Prevent runtime library mismatch when GoogleTest is built with MSVC.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_library(add STATIC
  src/add.c
  src/reversi.c
)
target_include_directories(add PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(app
  src/main.c
)
target_link_libraries(app PRIVATE add)

add_executable(add_tests
  tests/add_test.cpp
)
target_link_libraries(add_tests PRIVATE
  add
  GTest::gtest_main
)

gtest_discover_tests(add_tests)

if (MSVC)
    add_compile_options(/utf-8)
endif()
