#include "textureLoader.hpp"

//Checks if the current texture is loaded or not and loads it if not
int textureLoader::checkTextures(const char* Texture, bool atlas, bool minPixelated, bool magPixelated, bool MipMap, 
                                 int atlasdim, glm::vec2 atlasPoint) {
	
    for (const char* T : loadedTexturesNames) {
		if (Texture == T) { return std::distance(loadedTexturesNames.begin() 
			,std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture)); }
	}
	textureLoader::loadedTexturesNames.push_back(Texture);
	gl2d::Texture t;
	if (!atlas) {
		t.loadFromFile(Texture, minPixelated, magPixelated, MipMap);
	} else {
		gl2d::TextureAtlasPadding tPadding;
		t.loadFromFileWithPixelPadding(Texture, atlasdim, minPixelated, magPixelated, MipMap);
		tPadding = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, t.GetSize().x, t.GetSize().y);
		textureLoader::loadedTextureAtlases.push_back(tPadding);
	}
	textureLoader::loadedTextures.push_back(t);
	return std::distance(loadedTexturesNames.begin(), std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture));
}

