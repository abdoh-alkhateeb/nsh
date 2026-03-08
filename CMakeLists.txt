include_directories(include)
cmake_minimum_required(VERSION 3.10)
project(nsh)

set(CMAKE_CXX_STANDARD 17)

add_executable(nsh 
	src/main.cpp
	src/parser.cpp
	src/executer.cpp
	src/builtins.cpp
)

