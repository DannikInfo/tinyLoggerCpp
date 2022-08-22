cmake_minimum_required(VERSION 3.21)
project(tinyLogger)

set(CMAKE_CXX_STANDARD 23)

add_library(tinyLogger SHARED logger.cpp logger.h)
