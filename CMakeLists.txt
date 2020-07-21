cmake_minimum_required(VERSION 3.5)

include_directories(include)

file(GLOB SOURCES src/Cubemap/*.cpp src/Renderer/*.cpp render.cpp)
find_package (Threads REQUIRED)
add_executable(render ${SOURCES})
target_link_libraries (render ${CMAKE_THREAD_LIBS_INIT})