cmake_minimum_required(VERSION 3.12)

project(glad VERSION 1.0 LANGUAGES C)

add_library(glad src/glad.c)

target_include_directories(glad PUBLIC include)

target_link_libraries(glad)
