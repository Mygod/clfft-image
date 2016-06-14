cmake_minimum_required(VERSION 3.5)
project(parallel_computing)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES pfft.cc)
add_executable(pfft ${SOURCE_FILES})
target_link_libraries(pfft ${OpenCL_LIBRARY})
