cmake_minimum_required(VERSION 3.21)
project(tinyLogger)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_library(tinyLogger SHARED logger.cpp logger.h)

set_target_properties(tinyLogger PROPERTIES PUBLIC_HEADER logger.h)

install(TARGETS tinyLogger
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# Install target executable and .so files
install(FILES build/libtinyLogger.so DESTINATION /usr/local/lib)
install(FILES logger.h DESTINATION /usr/local/include)


set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "tinyLogger")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "A simple logger library")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dannikinfo")
include(CPack)