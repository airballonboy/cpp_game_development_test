#include "textureLoader.hpp"
#include <iostream>

//Checks if the current texture is loaded or not and loads it if not
int textureLoader::checkTextures(textureLoader::textureCTX ctx) {
	
    for (const char* T : loadedTexturesNames) {
		if (ctx.Texture == T) { return std::distance(loadedTexturesNames.begin() 
			,std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), ctx.Texture)); }
	}
	forceLoadTexture(ctx);
	return std::distance(loadedTexturesNames.begin(), std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), ctx.Texture));
}
void textureLoader::reloadTextures(){
	loadedTextures.clear();
	loadedAtlases.clear();
	for(const char* T : loadedTexturesNames){
		for (auto& ctx : textureCTXs){
			if (T == ctx.Texture) forceLoadTexture(ctx); 
		}
	}
	std::cout << "reloaded Textures" << std::endl;
}
void textureLoader::forceLoadTexture(textureLoader::textureCTX ctx) {

	textureLoader::loadedTexturesNames.push_back(ctx.Texture);
	gl2d::Texture t;
	if (!ctx.atlas) {
		t.loadFromFile(ctx.Texture, ctx.minPixelated, ctx.magPixelated, ctx.MipMap);
	} else {
		gl2d::TextureAtlasPadding tPadding;
		t.loadFromFileWithPixelPadding(ctx.Texture, ctx.atlasdim, ctx.minPixelated, ctx.magPixelated, ctx.MipMap);
		tPadding = gl2d::TextureAtlasPadding(ctx.atlasPoint.x, ctx.atlasPoint.y, t.GetSize().x, t.GetSize().y);
		textureLoader::loadedAtlases.push_back(tPadding);
	}
	textureLoader::loadedTextures.push_back(t);
	textureLoader::textureCTXs.push_back(ctx);
}
