cmake_minimum_required(VERSION 3.21)
project(SHELL VERSION 1.0)
include_directories(include)
add_executable(myapp src/main.cpp src/builtins.cpp src/executer.cpp src/parser.cpp)
