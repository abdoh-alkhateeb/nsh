cmake_minimum_required(VERSION 3.15)
project(nsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nsh
    src/main.cpp
    src/parser.cpp
    src/executer.cpp
    src/builtins.cpp
)

target_include_directories(nsh PRIVATE include)
