cmake_minimum_required(VERSION 3.21.2)
project (nsh VERSION 1.0 LANGUAGES CXX)

add_executable (nsh src/main.cpp src/executer.cpp src/parser.cpp src/builtins.cpp)

target_compile_options(nsh PRIVATE -Wall -O2)
target_compile_features(nsh PUBLIC cxx_std_17)

include_directories(//wsl.localhost/Ubuntu/usr/include/readline)
target_include_directories(nsh PRIVATE include)
target_link_libraries(nsh PRIVATE m readline)
