#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <string>
#define SDL_MAIN_HANDLED
#include <SDL.h>

class Window {
public:
	Window();
	~Window();
	Window(const Window&) = delete;

	bool initWindow(const std::string& title, glm::ivec2 windowPos, glm::ivec2 windowSize, SDL_WindowFlags windowFlags);
	bool initWindow(const std::string& title, glm::ivec2 windowSize, SDL_WindowFlags windowFlags);
	bool initOpenGl();
	bool initImGui(int imGuiFlags);

	void destroyWindow();
	void destroyOpenGl();
	void destroyImGui();

	void preDraw();
	void postDraw();

	uint32_t id();
	void handleImGuiEvent(const SDL_Event& event);

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	ImGuiIO imGuiIO;
};