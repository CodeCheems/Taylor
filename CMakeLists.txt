cmake_minimum_required(VERSION 3.21)
project(taylor)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/Framework/Common)
include_directories(${PROJECT_SOURCE_DIR}/Framework/Interface)

add_subdirectory(Framework)
add_subdirectory(Empty)
add_subdirectory(Platform)


