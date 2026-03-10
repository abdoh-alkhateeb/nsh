cmake_minimum_required(VERSION 3.21.2)
project(nsh)

add_executable(nsh
	src/main.cpp
	src/parser.cpp
	src/executer.cpp
	src/builtins.cpp)
target_include_directories(nsh PRIVATE include) 

