cmake_minimum_required(VERSION 3.24)
project(tema_lab2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tema_lab2
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        Set.cpp
        Set.h
        SetIterator.cpp
        SetIterator.h
        ShortTest.cpp
        ShortTest.h)
