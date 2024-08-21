cmake_minimum_required(VERSION 3.12)
project(SPL)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_definitions("-Wall" "-Wextra")

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

add_executable(splc ${SRC_FILES})

list(APPEND TEST_SRC_FILES ${SRC_FILES})
list(REMOVE_ITEM TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(splc_test ${TEST_SRC_FILES})

target_link_libraries(splc_test gtest_main)
target_link_libraries(splc_test gtest)

include(GoogleTest)
gtest_discover_tests(splc_test)
