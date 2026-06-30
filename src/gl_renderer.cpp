#include "gl_renderer.h"

#include <iostream>

#include <stb_image/stb_image.h>

int getCurrentOpenGLTexture(int textureIndex);

void initRenderer(Renderer *renderer, int width, int height)
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Init openGL
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &renderer->SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer->SSBO);
}

Texture registerTexture(Renderer *renderer, const char *name)
{
    Texture t;
    glGenTextures(1, &t.handle);
    // glActiveTexture(getCurrentOpenGLTexture(renderer->textures.size()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int channels;
    char buffer[512];
    const char *format = "assets/textures/%s";
    sprintf_s(buffer, format, name);

    unsigned char *data = stbi_load(buffer, &t.width, &t.height, &channels, 4);

    if (data)
    {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, t.width, t.height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    renderer->textures.add(t);
    return t;
}

void drawSprite(Renderer *renderer, Sprite *sprite, glm::vec2 pos, float scale)
{
    sprite->t.pos = pos;
    sprite->t.scale = scale;
    renderer->transforms.add(sprite->t);
}

void render(Renderer *renderer)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer->SSBO);

    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 renderer->transforms.size() * sizeof(Transform),
                 &renderer->transforms[0],
                 GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderer->SSBO);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderer->transforms.size());
    renderer->transforms.reset();
}

int getCurrentOpenGLTexture(int textureIndex)
{
    switch (textureIndex)
    {
    case 0:
        return GL_TEXTURE0;
    case 1:
        return GL_TEXTURE1;
    case 2:
        return GL_TEXTURE2;
    case 3:
        return GL_TEXTURE3;
    case 4:
        return GL_TEXTURE4;
    case 5:
        return GL_TEXTURE5;
    case 6:
        return GL_TEXTURE6;
    case 7:
        return GL_TEXTURE7;
    case 8:
        return GL_TEXTURE8;
    case 9:
        return GL_TEXTURE9;
    case 10:
        return GL_TEXTURE10;
    case 11:
        return GL_TEXTURE11;
    case 12:
        return GL_TEXTURE12;
    case 13:
        return GL_TEXTURE13;
    case 14:
        return GL_TEXTURE14;
    case 15:
        return GL_TEXTURE15;
    case 16:
        return GL_TEXTURE16;
    case 17:
        return GL_TEXTURE17;
    case 18:
        return GL_TEXTURE18;
    case 19:
        return GL_TEXTURE19;
    case 20:
        return GL_TEXTURE20;
    case 21:
        return GL_TEXTURE21;
    case 22:
        return GL_TEXTURE22;
    case 23:
        return GL_TEXTURE23;
    case 24:
        return GL_TEXTURE24;
    case 25:
        return GL_TEXTURE25;
    case 26:
        return GL_TEXTURE26;
    case 27:
        return GL_TEXTURE27;
    case 28:
        return GL_TEXTURE28;
    case 29:
        return GL_TEXTURE29;
    case 30:
        return GL_TEXTURE30;
    case 31:
        return GL_TEXTURE31;
    default:
        return -1;
    }
}
