cmake_minimum_required(VERSION 3.10)
project(nsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(nsh
    src/main.cpp
    src/parser.cpp
    src/executer.cpp
    src/builtins.cpp
)
