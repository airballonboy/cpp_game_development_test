#include "gameObject.h"
#include <gl2d/gl2d.h>
#include <iostream>



int objectCount = 0;
gl2d::TextureAtlasPadding bulletAtlas;
gl2d::Texture bulletTexture;

void loadOnceClass::loadBullets(gameObject::objectType type, const char* textureFile, gameObject::textureType _currentTextureType, glm::vec2 atlasPoint, glm::vec2 texturePoint, int atlasSize) {
	if (_currentTextureType == gameObject::normal) {
		bulletTexture.loadFromFile(textureFile, true);
	} else if (_currentTextureType == gameObject::atlas) {
		bulletTexture.loadFromFileWithPixelPadding(textureFile, atlasSize, true);
		bulletAtlas = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, bulletTexture.GetSize().x, bulletTexture.GetSize().y);
		printf("doen");
	}
}

void gameObject::createObject(objectType type, const char* textureFile, textureType _currentTextureType, glm::vec2 atlasPoint, glm::vec2 texturePoint, int atlasSize) {
	objectCount++;
	id = objectCount;
	currentAtlasPoint = texturePoint;
	currentTextureType = _currentTextureType;
	currentType = type;
	if (currentTextureType == normal) {
		objectTexture.loadFromFile(textureFile, true);
	} else if (currentTextureType == atlas) {
		objectTexture.loadFromFileWithPixelPadding(textureFile, atlasSize, true);
		objectAtlas = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, objectTexture.GetSize().x, objectTexture.GetSize().y);
	}
}
void gameObject::createObject(objectType type, textureType _currentTextureType, glm::vec2 atlasPoint, glm::vec2 texturePoint, int atlasSize) {
	objectCount++;
	id = objectCount;
	currentAtlasPoint = texturePoint;
	currentTextureType = _currentTextureType;
	currentType = type;
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


	if (currentType == bullet) {
		if (currentTextureType == normal) {
			renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, bulletTexture,
				Colors_White, {}, glm::degrees(this->rotation) + 90.f);
		} else if (currentTextureType == atlas) {
			renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, bulletTexture,
				Colors_White, {}, glm::degrees(this->rotation) + 90.f, bulletAtlas.get(currentAtlasPoint.x, currentAtlasPoint.y));
		}
		return;
	}
	if (currentTextureType == normal) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f);
	} else if (currentTextureType == atlas) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f, objectAtlas.get(currentAtlasPoint.x, currentAtlasPoint.y));
	}

}

