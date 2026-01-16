cmake_minimum_required(VERSION 3.14)
project(LazySequenceProject)

include_directories(include)

set(SOURCES src/main.cpp)
add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GRAPHVIZ REQUIRED libgvc)

target_link_libraries(main ${GRAPHVIZ_LIBRARIES})
target_include_directories(main PRIVATE ${GRAPHVIZ_INCLUDE_DIRS})
target_compile_options(main PRIVATE ${GRAPHVIZ_CFLAGS_OTHER})

find_package(GTest QUIET)
if(GTest_FOUND)
    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests GTest::gtest GTest::gtest_main)
endif()