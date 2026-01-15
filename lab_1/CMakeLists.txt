cmake_minimum_required(VERSION 3.14)
project(LazySequenceProject)

include_directories(include)

set(SOURCES
    src/main.cpp
)

add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)  

find_package(GTest QUIET)
if(GTest_FOUND)
    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests GTest::gtest GTest::gtest_main)
endif()