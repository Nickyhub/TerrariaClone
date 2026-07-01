#pragma once

#include <glm/glm.hpp>

typedef struct Transform
{
    glm::vec2 pos;
    glm::vec2 atlasPos;
    glm::vec2 tileSize;

    float scale;
    float padding; // necessary for alignment on GPU side
} Transform;

typedef enum ImageFileExtension
{
    JPG,
    PNG,
    BMP,

    IFE_MAX,
} ImageFileExtension;