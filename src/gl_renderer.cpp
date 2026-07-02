#include "gl_renderer.h"

#include <algorithm>
#include <iostream>

#include <stb_image/stb_image.h>

int getCurrentOpenGLTexture(int textureIndex);

static Renderer *render_ptr;

void initRenderer() {
	render_ptr = new Renderer();
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
						  GL_TRUE);

	// Init openGL
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &render_ptr->SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, render_ptr->SSBO);
}

void shutdownRenderer() { delete render_ptr; }

void registerTexture(const char *name, ImageFileExtension ext, int layer) {
	Texture t;
	glGenTextures(1, &t.handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int channels;
	char buffer[512];

	const char *extension;
	switch (ext) {
		case PNG:
			extension = "png";
			break;
		case JPG:
			extension = "jpg";
			break;
		case BMP:
			extension = "bmp";
			break;
		default:
			extension = "png";
			break;
	}

	const char *format_str = "assets/textures/%s.%s";
	sprintf_s(buffer, format_str, name, extension);

	unsigned char *data = stbi_load(buffer, &t.width, &t.height, &channels, 4);

	if (data) {
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, t.width, t.height, 0, format,
					 GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Put it in the atlas lookups and in the textures themselves
	render_ptr->textures.insert(std::make_pair(name, t));

	AtlasEntry ae;
	ae.name = name;
	ae.transforms = Array<Transform, MAX_TRANSFORMS>();
	ae.layer = layer;

	render_ptr->atlases.emplace_back(ae);
	std::sort(render_ptr->atlases.begin(), render_ptr->atlases.end(),
			  [](const AtlasEntry &a, const AtlasEntry &b) {
				  return a.layer < b.layer;
			  });
}

void drawSprite(std::string atlasName, Sprite *sprite, glm::vec2 pos,
				float scale) {
	sprite->t.pos = pos;
	sprite->t.scale = scale;
	for (int i = 0; i < render_ptr->atlases.size(); i++) {
		if (render_ptr->atlases[i].name == atlasName) {
			render_ptr->atlases[i].transforms.add(sprite->t);
		}
	}
}

void render() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, render_ptr->SSBO);

	glActiveTexture(GL_TEXTURE0);

	Renderer *r = render_ptr;
	for (int i = 0; i < r->atlases.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, r->textures.at(r->atlases[i].name).handle);

		glBufferData(GL_SHADER_STORAGE_BUFFER,
					 r->atlases[i].transforms.size() * sizeof(Transform),
					 &r->atlases[i].transforms[0], GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, render_ptr->SSBO);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
							  (GLsizei)r->atlases[i].transforms.size());
		r->atlases[i].transforms.reset();
	}
}
