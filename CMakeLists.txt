cmake_minimum_required(VERSION 3.22)
project(opengl_template)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(lib/sdl)

link_directories(lib/glm/glm)
include_directories(lib/glm)
add_subdirectory(lib/glm)

# Load Dear ImGui
add_library(ImGui STATIC
		# source
		lib/imgui/imgui.cpp
		lib/imgui/imgui_demo.cpp
		lib/imgui/imgui_draw.cpp
		lib/imgui/imgui_tables.cpp
		lib/imgui/imgui_widgets.cpp

		# cpp extension
		lib/imgui/misc/cpp/imgui_stdlib.cpp

		# backend
		lib/imgui/backends/imgui_impl_opengl3.cpp
		lib/imgui/backends/imgui_impl_sdl2.cpp
		)
target_link_libraries(ImGui PRIVATE SDL2::SDL2)
target_include_directories(ImGui PUBLIC lib/imgui lib/imgui/backends lib/imgui/misc/cpp)

add_subdirectory(lib/glad/cmake)
glad_add_library(glad_330 STATIC API gl:core=3.3)

include_directories(include)

set(SOURCES source/window.cpp source/shader.cpp source/shaderProgram.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ImGui Opengl32 glm glad_330)

# copy runtime dll to executable
if(WIN32)
	add_custom_command(
			TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" ${CMAKE_BINARY_DIR}
			VERBATIM
	)
endif()