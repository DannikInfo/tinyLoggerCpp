cmake_minimum_required(VERSION 3.21)
project(tinyLogger)

set(CMAKE_CXX_STANDARD 23)

add_library(tinyLogger SHARED logger.cpp logger.h)

set_target_properties(tinyLogger PROPERTIES PUBLIC_HEADER logger.h)

install(TARGETS tinyLogger
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})