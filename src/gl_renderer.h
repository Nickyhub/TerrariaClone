#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "render_types.h"
#include "array.h"

#define MAX_TEXTURES 32 // Max amount of possible openGL textures
#define MAX_TRANSFORMS 1000

typedef struct Texture
{
    GLuint handle;
    const char *name;
    int width, height;
} Texture;

typedef struct Renderer
{
    int width, height;
    GLuint VAO;
    GLuint SSBO;

    Array<Transform, MAX_TRANSFORMS> transforms;
    Array<Texture, MAX_TEXTURES> textures;
} Renderer;

typedef struct Sprite
{
    const char *name; // For debugging purposes for now
    Transform t;
} Sprite;

void initRenderer(Renderer *renderer, int width, int height);

Texture registerTexture(Renderer *renderer, const char *name);

void drawSprite(Renderer *renderer, Sprite *sprite, glm::vec2 pos, float scale);

void render(Renderer *renderer);