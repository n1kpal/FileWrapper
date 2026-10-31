cmake_minimum_required(VERSION 3.23)
project(FileWrapper CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(File STATIC src/File.cpp)

target_include_directories(File PUBLIC include)