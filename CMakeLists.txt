cmake_minimum_required(VERSION 3.21)
project(myapp LANGUAGES CXX)
add_executable(program src/main.cpp src/executer.cpp src/builtins.cpp src/parser.cpp)
target_include_directories(program PRIVATE ./include)