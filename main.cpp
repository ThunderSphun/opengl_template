#include <glad/gl.h>

#include "window.hpp"

void init();
void update();
void draw();
void destroy();

Window* window;
bool running = true;

int main() {
	init();

	while (running) {
		update();

		window->preDraw();
		draw();
		window->postDraw();
	}

	destroy();

	return 0;
}

void init() {
	window = new Window();
	if (!window->initWindow("Hello World", {720, 480}, SDL_WINDOW_OPENGL))
		exit(-1);
	if (!window->initOpenGl()) {
		window->destroyWindow();
		delete window;
		exit(-1);
	}
	if (!window->initImGui(ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable)) {
		window->destroyOpenGl();
		window->destroyWindow();
		delete window;
		exit(-1);
	}

	glm::vec4 clearColor(0.8, 0.4, 0.5, 1.0);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void update() {
	static SDL_Event event;

	while (SDL_PollEvent(&event)) {
		window->handleImGuiEvent(event);

		switch (event.type) {
			case SDL_QUIT: {
				running = false;
				break;
			}
			case SDL_KEYDOWN: {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
			}
			case SDL_WINDOWEVENT: {
				if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == window->id())
					running = false;
			}
		}
	}
}

void draw() {
	ImGui::ShowDemoWindow();
}

void destroy() {
	window->destroyImGui();
	window->destroyOpenGl();
	window->destroyWindow();
	delete window;
}