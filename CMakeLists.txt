cmake_minimum_required(VERSION 3.5)

find_package (Threads REQUIRED)
find_package (GTest REQUIRED)

# === src dependencies ===
include_directories(include)
include_directories(${GTEST_INCLUDE_DIRS})

# === test dependencies ===
enable_testing()
add_subdirectory("/usr/src/gtest" ${CMAKE_BINARY_DIR}/gtest)

add_subdirectory(test)

# === Add sources code ===
file(GLOB SOURCES src/Cubemap/*.cpp src/Renderer/*.cpp render.cpp)
add_executable(render ${SOURCES})
target_link_libraries (render ${CMAKE_THREAD_LIBS_INIT})