cmake_minimum_required(VERSION 3.13)
project(skeleton)

set(CMAKE_CXX_STANDARD 11)
add_executable(skeleton smash.cpp Commands.h signals.h )