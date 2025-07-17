cmake_minimum_required(VERSION 3.28)
project(MicroEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(MicroEngine STATIC game.cpp)

add_executable(gameExe game.cpp)