cmake_minimum_required(VERSION 3.15)
project(nsh)
include_directories(include)
add_executable(nsh src/main.cpp src/builtins.cpp src/executer.cpp src/parser.cpp)
