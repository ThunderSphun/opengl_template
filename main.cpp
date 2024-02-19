#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "window.hpp"
#include "shaderProgram.hpp"

void init();
void update();
void draw();
void destroy();

struct Vertex {
public:
	glm::vec3 position;
	glm::vec4 color;

	Vertex(const glm::vec3& position, const glm::vec4& color) : position(position), color(color) {}
};

Window* window;
bool running = true;
ShaderProgram* shaderProgram;

int modelViewUniform;
int timeUniform;

float rotation = 0;
double lastTime = 0;

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
	if (!window->initWindow("Hello World", {720, 480}, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
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

	Shader vertShader;
	Shader fragShader;

	try {
		vertShader = Shader("simple", GL_VERTEX_SHADER);
	} catch (const std::runtime_error& ex) {
		std::cerr << ex.what() << std::endl;
		delete window;
		exit(-1);
	}

	try {
		fragShader = Shader("simple", GL_FRAGMENT_SHADER);
	} catch (const std::runtime_error& ex) {
		std::cerr << ex.what() << std::endl;
		delete window;
		exit(-1);
	}

	shaderProgram = new ShaderProgram();
	shaderProgram
			->attachShader(vertShader)
			->attachShader(fragShader)
			->bindAttribute(0, "a_position")
			->bindAttribute(1, "a_color")
			->linkProgram()
			->useProgram();

	modelViewUniform = shaderProgram->getUniform("modelViewProjectionMatrix");
	timeUniform = shaderProgram->getUniform("time");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
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

	lastTime = (double) SDL_GetTicks64() / 1000.0;
	rotation = (float) lastTime;

	shaderProgram->update();
}

void draw() {
	ImGui::ShowDemoWindow();

	ImGui::Begin("backgroundColor");
	static glm::vec3 clearColor = glm::vec3(51, 51, 51) / 255.0f;
	ImGui::ColorEdit3("color", glm::value_ptr(clearColor));
	ImGui::End();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);

	glm::vec2 windowSize = window->getSize();
	glm::mat4 mvp = glm::perspective(glm::radians(80.0f), windowSize.x / windowSize.y, 0.01f, 100.0f);
	mvp *= glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvp = glm::translate(mvp, glm::vec3(0, 0, -1));
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelViewUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(timeUniform, (float) lastTime);

	Vertex vertices[] = {
			Vertex(glm::vec3(-1, -1, +1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(+1, -1, +1), glm::vec4(0, 1, 0, 1)),
			Vertex(glm::vec3(+1, +1, +1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, +1, +1), glm::vec4(0, 0, 1, 1)),

			Vertex(glm::vec3(+1, +1, -1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(-1, +1, -1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, -1, -1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(+1, -1, -1), glm::vec4(0, 1, 0, 1)),


			Vertex(glm::vec3(-1, +1, -1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(+1, +1, -1), glm::vec4(0, 1, 0, 1)),
			Vertex(glm::vec3(+1, +1, +1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, +1, +1), glm::vec4(0, 0, 1, 1)),

			Vertex(glm::vec3(+1, -1, +1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(-1, -1, +1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, -1, -1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(+1, -1, -1), glm::vec4(0, 1, 0, 1)),


			Vertex(glm::vec3(+1, -1, -1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(+1, +1, -1), glm::vec4(0, 1, 0, 1)),
			Vertex(glm::vec3(+1, +1, +1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(+1, -1, +1), glm::vec4(0, 0, 1, 1)),

			Vertex(glm::vec3(-1, +1, +1), glm::vec4(1, 0, 0, 1)),
			Vertex(glm::vec3(-1, -1, +1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, -1, -1), glm::vec4(0, 0, 1, 1)),
			Vertex(glm::vec3(-1, +1, -1), glm::vec4(0, 1, 0, 1)),
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), vertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &vertices[0].color);
	glDrawArrays(GL_QUADS, 0, 24);
}

void destroy() {
	window->destroyImGui();
	window->destroyOpenGl();
	window->destroyWindow();
	delete window;
}