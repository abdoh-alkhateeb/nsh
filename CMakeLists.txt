cmake_minimum_required(VERSION 3.21)
project(nsh)

include_directories(include)
add_executable(nsh src/main.cpp src/builtins.cpp src/parser.cpp src/executer.cpp)
