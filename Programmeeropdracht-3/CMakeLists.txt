cmake_minimum_required(VERSION 3.26)
project(Programmeeropdracht_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Programmeeropdracht_3
        DFA.h
        json.hpp
        main.cpp
        SA.cpp
        SA.h
)
