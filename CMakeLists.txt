cmake_minimum_required(VERSION 3.10)

project(similarity-search)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC src/*.cpp src/*.h)

add_executable(ssearch ${SRC})
