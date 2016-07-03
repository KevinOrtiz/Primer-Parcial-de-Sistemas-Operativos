cmake_minimum_required(VERSION 3.5)
project(HashEstructure)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Hash.c dbg.cpp dbg.h bstrlib.cpp bstrlib.h bstrlib.c darray.c darray.h principal.c)
add_executable(HashEstructure ${SOURCE_FILES})