#pragma once

#include <glm/glm.hpp>

typedef struct Transform {
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 atlasPos;
	glm::vec2 tileSize;
} Transform;

typedef enum ImageFileExtension {
	JPG,
	PNG,
	BMP,

	IFE_MAX,
} ImageFileExtension;