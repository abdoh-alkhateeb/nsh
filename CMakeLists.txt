cmake_minimum_required(VERSION 3.21)
project(myapp LANGUAGES CXX)

add_executable(myapp src/main.cpp)
add_library(mylib STATIC src/parser.cpp src/builtins.cpp src/executer.cpp)

target_include_directories(mylib PUBLIC include/)
target_link_libraries(myapp PRIVATE mylib)
