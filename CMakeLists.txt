cmake_minimum_required(VERSION 3.21)
project(assignment)
add_executable(myprog
    src/main.cpp
    src/builtins.cpp
    src/executer.cpp
    src/parser.cpp
)
target_include_directories(myprog PRIVATE include)
