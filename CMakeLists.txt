cmake_minimum_required(VERSION 4.0)
project(nsh)
set(CMAKE_CXX_STANDARD 20)

add_executable(nsh
        src/main.cpp
        src/builtins.cpp
        src/executer.cpp
        src/parser.cpp
)

target_include_directories(nsh PRIVATE include src)