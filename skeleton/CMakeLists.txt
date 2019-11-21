cmake_minimum_required(VERSION 3.9)
project(OS)

set(CMAKE_CXX_STANDARD 11)

add_executable(OS smash.cpp Commands.h Commands.cpp signals.cpp signals.h)