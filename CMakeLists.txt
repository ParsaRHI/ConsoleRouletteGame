cmake_minimum_required(VERSION 3.28)
project(ConsoleGame)

set(CMAKE_CXX_STANDARD 23)

add_executable(ConsoleGame main.cpp
        color.hpp
        item.h
        GRand.h)
