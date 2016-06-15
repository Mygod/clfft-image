cmake_minimum_required(VERSION 3.5)
project(parallel_computing)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED COMPONENTS date_time filesystem)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})

find_package(clFFT REQUIRED)
include_directories(${CLFFT_INCLUDE_DIRS})

find_package(png++ REQUIRED)
include_directories(${png++_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -D__CL_ENABLE_EXCEPTIONS -lclFFT")

set(SOURCE_FILES CLContext.h CLContext.cc CLPlatform.h CLPlatform.cc CLFFT.h CLFFT.cc
        PixelArray.h FileReader.h FileReader.cc pfft.cc)
add_executable(pfft ${SOURCE_FILES})
target_link_libraries(pfft ${Boost_DATE_TIME_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES} ${Boost_SYSTEM_LIBRARIES}
        ${OpenCL_LIBRARIES} ${CLFFT_LIBRARIES} ${png++_LIBRARIES})

add_custom_command(TARGET pfft PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/input.png $<TARGET_FILE_DIR:pfft>/input.png
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/kernel.cl $<TARGET_FILE_DIR:pfft>/kernel.cl)
