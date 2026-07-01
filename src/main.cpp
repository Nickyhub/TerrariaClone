#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>

#include <iostream>
#include <vector>

#include "gl_renderer.h"
#include "render_types.h"
#include "shader.h"

const int width = 1920, height = 1080;

void onWindowResize(GLFWwindow *window) {
	// glViewport(get)
}

int main(void) {
	if (!glfwInit())
		return -1;

	// enable opengl debugging output.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
				   GLFW_OPENGL_CORE_PROFILE); // you might want to do this when
											  // testing the game for shipping

	GLFWwindow *window = window =
		glfwCreateWindow(width, height, "Terraria", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!result) {
		std::cerr << "Failed to load glad." << std::endl;
	}

	glfwSetWindowRefreshCallback(window, onWindowResize);

	glfwSwapInterval(1);

	initRenderer();

	registerTexture("Forest", PNG);
	registerTexture("splash", PNG);

	std::cout << "Size of Transorm: " << sizeof(Transform) << std::endl;

	// Set up transforms for the shader
	std::vector<Transform> transforms;
	Transform t;
	t.atlasPos.x = 1; // Should be the dirt tile Forest.png
	t.atlasPos.y = 1;
	t.tileSize.x = 8;
	t.tileSize.y = 10;

	t.pos.x = 100;
	t.pos.y = 100;

	t.scale = 1.0;

	transforms.push_back(t);
	t.pos.x = 200;
	transforms.push_back(t);

	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

	glBufferData(GL_SHADER_STORAGE_BUFFER,
				 transforms.size() * sizeof(Transform), &transforms[0],
				 GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	glm::mat4 ortho =
		glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	Shader s("assets/shaders/material.vert.glsl",
			 "assets/shaders/material.frag.glsl");
	s.use();
	s.setInt("backgroundTexture", 0);
	s.setMatrix4f("ortho", ortho);

	Sprite sp;
	sp.name = "Forest";
	sp.t = t;

	Sprite background;
	background.name = "Background";
	t.atlasPos = glm::vec2(0.0f, 0.0f);
	t.pos = glm::vec2(0.0f, 0.0f);
	t.tileSize = glm::vec2(1920.0f, 1080.0f);
	background.t = t;

	glViewport(0, 0, width, height);
	while (!glfwWindowShouldClose(window)) {
		int key = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (key == GLFW_PRESS) {
			break;
		}

		glClearColor(0.0f, 0.3f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		s.use();
		s.setMatrix4f("ortho", ortho);

		drawSprite("splash", &background, glm::vec2(0, 0), 1.0f);
		// drawSprite("Forest", &sp, glm::vec2(100, 200), 5.0f);
		// drawSprite("Forest", &sp, glm::vec2(100, 300), 5.0f);
		// drawSprite("Forest", &sp, glm::vec2(200, 200), 5.0f);
		// drawSprite("Forest", &sp, glm::vec2(300, 200), 5.0f);
		// drawSprite("Forest", &sp, glm::vec2(400, 200), 5.0f);
		// drawSprite("Forest", &sp, glm::vec2(500, 200), 5.0f);

		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shutdownRenderer();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
