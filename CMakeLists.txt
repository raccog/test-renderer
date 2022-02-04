cmake_minimum_required(VERSION 3.12)

project(renderer VERSION 0.1.0
    LANGUAGES CXX)

add_subdirectory(libs)

add_executable(renderer src/main.cpp src/camera.cpp src/shaders.cpp src/utils.cpp)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(renderer PUBLIC _DEBUG)
	if ((NOT IS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders) OR (${REFRESH_SHADERS}))
        execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/shaders
                        COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/basic.vs
                        COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/basic.fs
                        COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/shaders/lighting.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/lighting.fs
						)
    endif()
else()
    file(COPY
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.vs
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic.fs
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/lighting.fs
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)
endif()

target_include_directories(renderer PUBLIC src)
set_target_properties(renderer PROPERTIES CXX_STANDARD 17)

target_link_libraries(renderer glad glfw glm spdlog)
