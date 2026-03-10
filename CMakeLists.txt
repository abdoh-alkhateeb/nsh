cmake_minimum_required(VERSION 3.21.2)
project(HW1 VERSION 1.0 LANGUAGES CXX)

include_directories(include)

add_executable(HW1
    src/main.cpp
    src/builtins.cpp
    src/executer.cpp
    src/parser.cpp)

