#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>

#include "array.h"
#include "render_types.h"

#define MAX_TEXTURES 32 // Max amount of possible openGL textures
#define MAX_TRANSFORMS 1000

typedef struct Texture {
	int width, height;
	std::string name;
	GLuint handle;
} Texture;

typedef struct AtlasEntry {
	std::string name;
	Array<Transform, MAX_TRANSFORMS> transforms;
	int layer;
} AtlasEntry;

typedef struct Renderer {
	int width, height;
	GLuint VAO;
	GLuint SSBO;

	std::vector<AtlasEntry> atlases;

	// Array<Transform, MAX_TRANSFORMS> transforms;
	std::map<std::string, Texture> textures;
} Renderer;

typedef struct Sprite {
	const char *name; // For debugging purposes for now
	Transform t;
} Sprite;

void initRenderer();
void shutdownRenderer();

void registerTexture(const char *name, ImageFileExtension ext, int layer);

void drawSprite(std::string atlasName, Sprite *sprite, glm::vec2 pos,
				float scale);

void render();