cmake_minimum_required(VERSION 3.6)
project(VEBTree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES veb.cpp veb.h main.cpp)
add_executable(VEBTree ${SOURCE_FILES})