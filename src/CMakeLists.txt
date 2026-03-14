cmake_minimum_required(VERSION 3.10)
project(nsh VERSION 1.1 LANGUAGES CXX)
add_executable(nsh
    main.cpp
    parser.cpp
    executer.cpp
    builtins.cpp
)
target_compile_features(nsh PRIVATE cxx_std_17)
target_include_directories(nsh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
