cmake_minimum_required(VERSION 3.6)
project(portal)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    mainwindow.h)

add_executable(portal ${SOURCE_FILES})
