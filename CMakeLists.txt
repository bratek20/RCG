cmake_minimum_required (VERSION 2.6)

project (RCG)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(src include)

add_subdirectory(src)

