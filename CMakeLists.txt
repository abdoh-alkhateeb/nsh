cmake_minimum_required(VERSION 3.10)
project(MyShell)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyShell
    main.cpp
    shell.cpp
    shell.h
)

