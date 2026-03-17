cmake_minimum_required(VERSION 3.21)
project(zad)
add_executable(myshell src/builtins.cpp  src/executer.cpp  src/main.cpp src/parser.cpp)
target_include_directories(myshell PRIVATE  include)
