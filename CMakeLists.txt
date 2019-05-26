cmake_minimum_required(VERSION 3.0.0)
project(steg VERSION 0.1.0)

if (WIN32)
    set(OpenCV_DIR "C:/opencv/mingw-build/install")
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11")

set(SOURCES
        main.cpp
        encode.hpp
        encode.cpp
        decode.hpp
        decode.cpp
        steg.hpp
        )

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})