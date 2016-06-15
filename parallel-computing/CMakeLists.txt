cmake_minimum_required(VERSION 3.5)
project(parallel_computing)

find_package(Boost REQUIRED COMPONENTS date_time filesystem)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES CLContext.h CLContext.cc CLPlatform.h CLPlatform.cc pfft.cc)
add_executable(pfft ${SOURCE_FILES})
target_link_libraries(pfft ${Boost_DATE_TIME_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES} ${Boost_SYSTEM_LIBRARIES}
        ${OpenCL_LIBRARY})

add_custom_command(TARGET pfft POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/kernel.cl $<TARGET_FILE_DIR:pfft>/kernel.cl)
