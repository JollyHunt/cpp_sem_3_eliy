cmake_minimum_required(VERSION 3.14)
project(LazySequenceProject)

include_directories(include)

add_executable(cli src/cli.cpp)

add_executable(han src/hand_test_ls.cpp)              

# add_executable(main src/main.cpp)    

find_package(GTest QUIET)
if(GTest_FOUND)
    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests GTest::gtest GTest::gtest_main)
endif()