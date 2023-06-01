cmake_minimum_required(VERSION 3.16)
project("Thread Pool" C CXX)

add_subdirectory(dstruct)

add_library(tpool SHARED ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.c)
target_include_directories(tpool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tpool dstruct_static pthread)