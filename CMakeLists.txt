cmake_minimum_required(VERSION 3.10)
project(nsh VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(nsh
    src/main.cpp
    src/parser.cpp
    src/executer.cpp
    src/builtins.cpp
    src/history.cpp
)
