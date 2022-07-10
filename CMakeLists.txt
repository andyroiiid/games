cmake_minimum_required(VERSION 3.22)
project(games)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(engine)

add_subdirectory(proto2d)