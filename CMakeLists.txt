cmake_minimum_required(VERSION 3.16)
project(OOP_Lab4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(geometry INTERFACE)
target_include_directories(geometry INTERFACE include)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE geometry)

add_executable(tests
    tests/test_main.cpp
    tests/test_figures.cpp
    tests/test_array.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE geometry gtest_main gmock_main)

target_compile_definitions(tests PRIVATE GTEST_HAS_TR1_TUPLE=0)

enable_testing()
add_test(NAME AllTests COMMAND tests)