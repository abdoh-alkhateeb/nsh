cmake_minimum_required(VERSION 3.10)

project(nsh)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(nsh src/main.cpp src/parser.cpp src/executer.cpp src/builtins.cpp)
