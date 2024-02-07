#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

int main() {
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_OPENGL);

	bool running = true;

	while (running) {
		static SDL_Event event;

		while (SDL_PollEvent(&event)) {
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
			}
		}
	}

	SDL_DestroyWindow(window);

	return 0;
}
