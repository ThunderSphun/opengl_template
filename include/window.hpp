#pragma once

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

	bool initWindow(const std::string& title, glm::ivec2 windowPos, glm::ivec2 windowSize, int windowFlags);
	bool initWindow(const std::string& title, glm::ivec2 windowSize, int windowFlags);
	bool initOpenGl();
	bool initImGui(int imGuiFlags);

	void destroyWindow();
	void destroyOpenGl();
	void destroyImGui();

	void preDraw() const;
	void postDraw() const;

	[[nodiscard]] uint32_t id() const;
	void handleImGuiEvent(const SDL_Event& event) const;
	[[nodiscard]] glm::ivec2 getSize() const;

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	ImGuiIO* imGuiIO;
};