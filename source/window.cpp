#include "window.hpp"

#include <iostream>
#include <glad/gl.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

Window::Window() {
	window = nullptr;
	glContext = nullptr;
}

Window::~Window() {
	destroyImGui();
	destroyOpenGl();
	destroyWindow();
}

bool Window::initWindow(const std::string& title, glm::ivec2 windowSize, int windowFlags) {
	return initWindow(title, {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, windowSize, windowFlags);
}

bool Window::initWindow(const std::string& title, glm::ivec2 windowPos, glm::ivec2 windowSize, int windowFlags) {
	if (window != nullptr) {
		std::cout << "attempted to reinitialize a window" << std::endl;
		return false;
	}

	if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow(title.c_str(), windowPos.x, windowPos.y, windowSize.x, windowSize.y, windowFlags);
	if (window == nullptr) {
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	if ((windowFlags & SDL_WINDOW_OPENGL) == 0)
		if (glContext) // remove context if it somehow exists
			SDL_GL_DeleteContext(glContext);

	return true;
}

bool Window::initOpenGl() {
	if (window == nullptr) {
		std::cout << "attempted to initialize opengl without a window" << std::endl;
		return false;
	}
	if (glContext != nullptr) {
		std::cout << "attempted to reinitialize opengl context" << std::endl;
		return false;
	}

	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		destroyWindow();

		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	{
		int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
		if (version == 0) {
			destroyWindow();

			std::cerr << "glad could not load opengl functions" << std::endl;
			return false;
		}
	}

	if (SDL_GL_MakeCurrent(window, glContext) != 0) {
		destroyWindow();
		destroyOpenGl();

		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	{
		int retVal = SDL_GL_SetSwapInterval(-1); // attempt to set adaptive vsync
		if (retVal == 0)
			return true;

		retVal = SDL_GL_SetSwapInterval(1);
		if (retVal == -1) {
			destroyWindow();
			destroyOpenGl();

			std::cerr << SDL_GetError() << std::endl;
			return false;
		}
	}

	return true;
}

bool Window::initImGui(const int imGuiFlags) {
	if (!IMGUI_CHECKVERSION()) {
		std::cerr << "Dear ImGui has the wrong version" << std::endl;
		return false;
	}
	ImGui::CreateContext();
	imGuiIO = ImGui::GetIO();

	imGuiIO.ConfigFlags |= imGuiFlags;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (imGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	if (window == nullptr) {
		destroyImGui();

		std::cerr << "SDL window not yet initialized" << std::endl;
		return false;
	}

	if (glContext == nullptr) {
		destroyImGui();

		std::cerr << "SDL window not initialized with 'SDL_WINDOW_OPENGL'" << std::endl;
		return false;
	}
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	return true;
}

void Window::destroyWindow() {
	if (window == nullptr)
		SDL_DestroyWindow(window);

	window = nullptr;
	if (SDL_WasInit(0) != 0)
		SDL_Quit();
}

void Window::destroyOpenGl() {
	if (glContext == nullptr)
		SDL_GL_DeleteContext(glContext);

	glContext = nullptr;
}

void Window::destroyImGui() {
	if (window == nullptr && glContext == nullptr) // assume window is already closed
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Window::preDraw() const {
	if (window == nullptr && glContext == nullptr) // assume window is already closed
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	glm::ivec2 windowSize = getSize();
	glViewport(0, 0, windowSize.x, windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::postDraw() const {
	if (window == nullptr && glContext == nullptr) // assume window is already closed
		return;

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if ((imGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	SDL_GL_SwapWindow(window);
}

uint32_t Window::id() const {
	return SDL_GetWindowID(window);
}

void Window::handleImGuiEvent(const SDL_Event& event) const {
	if (window == nullptr && glContext == nullptr) // assume window is already closed
		return;

	ImGui_ImplSDL2_ProcessEvent(&event);
}

glm::ivec2 Window::getSize() const {
	glm::ivec2 windowSize(0, 0);
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
	return windowSize;
}
