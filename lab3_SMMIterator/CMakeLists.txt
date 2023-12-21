cmake_minimum_required(VERSION 3.24)
project(tema_lab3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tema_lab3
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ShortTest.cpp
        ShortTest.h
        SMMIterator.cpp
        SMMIterator.h
        SortedMultiMap.cpp
        SortedMultiMap.h)
