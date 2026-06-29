#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "shader.h"

typedef struct Transform
{
	glm::vec2 atlasPos;
	glm::vec2 pos;
	glm::vec2 tileSize;

	float scale; // scales x and y both
} Transform;

const int width = 1920,
		  height = 1080;

int main(void)
{

	if (!glfwInit())
		return -1;

	// enable opengl debugging output.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // you might want to do this when testing the game for shipping

	GLFWwindow *window = window = glfwCreateWindow(width, height, "Terraria", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!result)
	{
		std::cerr << "Failed to load glad." << std::endl;
	}

	glfwSwapInterval(1);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	unsigned int backgroundTex;
	glGenTextures(1, &backgroundTex);
	glBindTexture(GL_TEXTURE_2D, backgroundTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texWidth, texHeight, channels;
	stbi_set_flip_vertically_on_load(false); // wichtig: OpenGL erwartet UV-Ursprung unten links
	unsigned char *backgroundTexture = stbi_load("assets/textures/background.png", &texWidth, &texHeight, &channels, 0);

	if (backgroundTexture)
	{
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, backgroundTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(backgroundTexture);

	GLuint spriteSheet;
	glGenTextures(1, &spriteSheet);
	glBindTexture(GL_TEXTURE_2D, spriteSheet);

	unsigned char *spriteSheetTexture = stbi_load("assets/textures/Forest.png", &texWidth, &texHeight, &channels, 0);
	if (spriteSheet)
	{
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, spriteSheetTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(spriteSheetTexture);

	// Set up transforms for the shader
	std::vector<Transform> transforms;
	Transform t;
	t.atlasPos.x = 1; // Should be the dirt tile Forest.png
	t.atlasPos.y = 1;
	t.tileSize.x = 8;
	t.tileSize.y = 10;

	t.pos.x = 100;
	t.pos.y = 100;

	t.scale = 5.0;
	transforms.push_back(t);

	GLuint ssbo;
	glGenBuffers(1, &ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
				 transforms.size() * sizeof(Transform),
				 transforms.data(),
				 GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	// Init openGL
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glm::mat4 ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	Shader s("assets/shaders/material.vert.glsl", "assets/shaders/material.frag.glsl");
	s.use();
	s.setInt("backgroundTexture", 0);
	s.setMatrix4f("ortho", ortho);

	glViewport(0, 0, width, height);
	while (!glfwWindowShouldClose(window))
	{
		int key = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (key == GLFW_PRESS)
		{
			break;
		}

		glClearColor(0.0f, 0.3f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		s.use();
		s.setMatrix4f("ortho", ortho);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backgroundTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spriteSheet);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, transforms.size());
		// glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
