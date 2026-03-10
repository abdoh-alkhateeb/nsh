cmake_minimum_required(VERSION 3.21.2)

project(linuxShell VERSION 1.0 LANGUAGES CXX)

add_library(linuxLibrary STATIC ./src/builtins.cpp ./src/executer.cpp ./src/parser.cpp )
add_executable(main ./src/main.cpp)

target_compile_options(linuxLibrary PRIVATE -Wall)
target_compile_options(main PRIVATE -Wall)

target_compile_features(linuxLibrary PUBLIC cxx_std_17)
target_compile_features(main PUBLIC cxx_std_17)

target_link_libraries(main PRIVATE linuxLibrary)
target_include_directories(main PRIVATE include)
target_include_directories(linuxLibrary PRIVATE include)
