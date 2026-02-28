cmake_minimum_required(VERSION 3.21)

project(nsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(nsh ${SRC_FILES})

target_include_directories(nsh PRIVATE include)

target_compile_options(nsh PRIVATE -Wall)
