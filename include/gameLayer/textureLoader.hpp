#pragma once
#include <vector>
#include "gl2d/gl2d.h"

class textureLoader{
private:

public:
	std::vector<const char*> loadedTexturesNames;
	std::vector<gl2d::Texture> loadedTextures;
	std::vector<gl2d::TextureAtlasPadding> loadedTextureAtlases;

	int checkTextures(const char*, bool, bool = false, bool = true, bool = true, int = 128, glm::vec2 = { 0, 0 });

};
