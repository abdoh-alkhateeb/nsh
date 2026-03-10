cmake_minimum_required(VERSION 3.10)

project(MyProgram)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProgram
    main.cpp
    builtins.cpp
    executer.cpp
    parser.cpp
)
