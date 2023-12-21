cmake_minimum_required(VERSION 3.24)
project(tema_lab5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tema_lab5
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ShortTest.cpp
        ShortTest.h
        SMIterator.cpp
        SMIterator.h
        SortedMap.cpp
        SortedMap.h)
