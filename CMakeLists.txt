cmake_minimum_required(VERSION 3.22)
project(opengl_template)

set(CMAKE_CXX_STANDARD 23)

# Load SDL2
add_subdirectory(lib/sdl)

include_directories(include)

add_executable(opengl_template main.cpp)

# Link SDL2
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)

# copy runtime dll to executable
if(WIN32)
	add_custom_command(
			TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" ${CMAKE_BINARY_DIR}
			VERBATIM
	)
endif()