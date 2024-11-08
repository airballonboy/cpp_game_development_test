#include "gameObject.h"
#include <gl2d/gl2d.h>
#include <iostream>



int objectCount = 0;
gl2d::TextureAtlasPadding bulletAtlas;
gl2d::Texture bulletTexture;
loadOnceClass loadOnce;

//vectors are a pain
int loadOnceClass::checkTextures(const char* Texture, bool atlas, int atlasSize, glm::vec2 atlasPoint) {
	for (const char* T : loadedTexturesNames) {
		if (Texture == T) { return std::distance(loadedTexturesNames.begin(), find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture)); }
	}
	loadOnceClass::loadedTexturesNames.push_back(Texture);
	gl2d::Texture t;
	gl2d::TextureAtlasPadding tPadding;
	if (!atlas) {
		t.loadFromFile(Texture, true);
	} else {
		t.loadFromFileWithPixelPadding(Texture, atlasSize, true);
		tPadding = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, t.GetSize().x, t.GetSize().y);
		loadOnceClass::loadedTextureAtlases.push_back(tPadding);
	}
	loadOnceClass::loadedTextures.push_back(t);
	return std::distance(loadedTexturesNames.begin(), find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture));
}

void gameObject::createObject(objectType type, const char* textureFile, textureType _currentTextureType, glm::vec2 atlasPoint, glm::vec2 texturePoint, int atlasSize) {
	objectCount++;
	id = objectCount;
	currentAtlasPoint = texturePoint;
	currentTextureType = _currentTextureType;
	currentType = type;
	if (currentTextureType == normal) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(textureFile, false)];
	} else if (currentTextureType == atlas) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(textureFile, true, atlasSize, atlasPoint)];
		objectAtlas = loadOnce.loadedTextureAtlases[loadOnce.checkTextures(textureFile, true, atlasSize, atlasPoint)];
	}
}


bool gameObject::isTheSameObject(gameObject otherObject) {
	if (this->id == otherObject.id) {
		return true;
	} else { return false; }
}

int gameObject::getId() {
	return this->id;
}

void gameObject::setSize(float newSizeX, float newSizeY) {
	size = { newSizeX, newSizeY };
	center = { size.x / 2, size.y / 2 };
}

void gameObject::gravity() {
	//TODO add collision detection and gravity system
}
void gameObject::move(float deltaTime) {
	movement = glm::normalize(movement);
	movement += movement * deltaTime * speed;
}


void gameObject::update(float deltaTime, gl2d::Renderer2D& renderer) {
	if (enableGravity) { this->gravity(); }
	if (movement != glm::vec2{0, 0}) { this->move(deltaTime); this->pos += movement; }


	/*if (currentType == bullet) {
		if (currentTextureType == normal) {
			renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, bulletTexture,
				Colors_White, {}, glm::degrees(this->rotation) + 90.f);
		} else if (currentTextureType == atlas) {
			renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, bulletTexture,
				Colors_White, {}, glm::degrees(this->rotation) + 90.f, bulletAtlas.get(currentAtlasPoint.x, currentAtlasPoint.y));
		}
		return;
	}*/
	if (currentTextureType == normal) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f);
	} else if (currentTextureType == atlas) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f, objectAtlas.get(currentAtlasPoint.x, currentAtlasPoint.y));
	}

}

