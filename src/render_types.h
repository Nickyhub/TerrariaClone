#pragma once

#include <glm/glm.hpp>

typedef struct Transform
{
    glm::vec2 pos;
    glm::vec2 atlasPos;
    glm::vec2 tileSize;
} Transform;