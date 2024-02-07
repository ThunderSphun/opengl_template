#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

int main() {
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_RESIZABLE);

	SDL_Delay(5000);

	SDL_DestroyWindow(window);

	return 0;
}
