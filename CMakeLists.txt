cmake_minimum_required(VERSION 3.10)
project(nsh)
set(CMAKE_CXX_STANDARD 17)
add_executable(nsh
main.cpp
parser.cpp
executer.cpp
builtin.cpp
)
