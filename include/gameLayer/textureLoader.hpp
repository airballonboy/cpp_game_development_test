#pragma once
#include <vector>
#include "gl2d/gl2d.h"

class textureLoader{
public:
	struct textureCTX{
		const char* Texture;
		bool atlas = false;
		bool minPixelated = false;
		bool magPixelated = true;
		bool MipMap = true;
		int atlasdim= 128; 
		glm::vec2 atlasPoint = { 0, 0 };
	};

	std::vector<const char*> loadedTexturesNames;
	std::vector<gl2d::Texture> loadedTextures;
	std::vector<gl2d::TextureAtlasPadding> loadedAtlases;
	std::vector<textureCTX> textureCTXs;

	int checkTextures(textureCTX);
	void reloadTextures();

private:
	void forceLoadTexture(textureCTX);

};
