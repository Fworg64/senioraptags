cmake_minimum_required(VERSION 2.6)


project(SENIORAPTAGS)


set(EXECUTABLE_OUTPUT_PATH ./)
set(LIBRARY_OUTPUT_PATH ./)



find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-Wall -g")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare -frounding-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare -frounding-math")

set(CMAKE_C_FLAGS_DEBUG "-O")
set(CMAKE_CXX_FLAGS_DEBUG "-O")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


add_subdirectory(src)
