cmake_minimum_required(VERSION 2.8)
project(Centers)
find_package(OpenCV REQUIRED )
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
set( NAME_SRC
    main.cpp
)
set( NAME_HEADERS
    # include/header.h
)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
#link_directories( ${CMAKE_BINARY_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable( Centers ${NAME_SRC} ${NAME_HEADERS} )
target_link_libraries( Centers ${OpenCV_LIBS} )
