cmake_minimum_required(VERSION 3.12)

project(renderer VERSION 0.1.0
    LANGUAGES CXX)

add_subdirectory(libs)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.vs
	 ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.fs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_executable(renderer src/main.cpp src/shaders.cpp src/utils.cpp)

target_include_directories(renderer PUBLIC src)
set_target_properties(renderer PROPERTIES CXX_STANDARD 17)

target_link_libraries(renderer glad glfw glm)
