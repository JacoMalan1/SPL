cmake_minimum_required(VERSION 3.12)
project(SPL)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(splc src/main.cpp src/lexer.cpp src/token.cpp)
add_executable(splc_test tests/lexer_test.cpp src/lexer.cpp src/token.cpp)

target_include_directories(splc PUBLIC include)
target_include_directories(splc_test PUBLIC include)

target_link_libraries(splc_test gtest_main)
target_link_libraries(splc_test gtest)

include(GoogleTest)
gtest_discover_tests(splc_test)
