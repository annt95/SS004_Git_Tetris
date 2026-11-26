cmake_minimum_required(VERSION 3.16)
project(SS004_Git_Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tetris_board src/board.cpp)
target_include_directories(tetris_board PUBLIC src)

add_executable(tetris src/main.cpp)
target_link_libraries(tetris PRIVATE tetris_board)

include(CTest)
if(BUILD_TESTING)
    add_executable(tetris_tests tests/board_tests.cpp)
    target_link_libraries(tetris_tests PRIVATE tetris_board)
    add_test(NAME board_tests COMMAND tetris_tests)
endif()
