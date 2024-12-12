#include "gameObject.hpp"
#include <cstring>
#include <gl2d/gl2d.h>
#include <algorithm>



int objectCount = 0;
loadOnceClass loadOnce;



//Checks if the current texture is loaded or not and loads it if not
int loadOnceClass::checkTextures(const char* Texture, bool atlas, int atlasdim, glm::vec2 atlasPoint) {
	for (const char* T : loadedTexturesNames) {
		if (Texture == T) { return std::distance(loadedTexturesNames.begin() 
			,std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture)); }
	}
	loadOnceClass::loadedTexturesNames.push_back(Texture);
	gl2d::Texture t;
	if (!atlas) {
		t.loadFromFile(Texture, true);
	} else {
		gl2d::TextureAtlasPadding tPadding;
		t.loadFromFileWithPixelPadding(Texture, atlasdim, true);
		tPadding = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, t.GetSize().x, t.GetSize().y);
		loadOnceClass::loadedTextureAtlases.push_back(tPadding);
	}
	loadOnceClass::loadedTextures.push_back(t);
	return std::distance(loadedTexturesNames.begin(), std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture));
}
int gameObject::getObjectCount() {
	return objectCount;
}


//Creating the object and loading it's texture if it isn't loaded
void gameObject::createObject(objectType _type, const char* _textureFile, textureType _currentTextureType
			      , glm::vec2 _atlasPoint, glm::vec2 _texturePoint, int _atlasdim) {

	objectCount++;
	id = objectCount;
	currentAtlasPoint = _texturePoint;
	currentTextureType = _currentTextureType;
	currentType = _type;
	if (currentTextureType == normal) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(_textureFile, false)];
	} else if (currentTextureType == atlas) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(_textureFile, true, _atlasdim, _atlasPoint)];
		objectAtlas = loadOnce.loadedTextureAtlases[loadOnce.checkTextures(_textureFile, true, _atlasdim, _atlasPoint)];
	}
}

//Checks if two objects are the same
bool gameObject::isTheSameObject(gameObject otherObject) {
	if (this->id == otherObject.id) {
		return true;
	} else { return false; }
}

int gameObject::getId() {
	return this->id;
}

void gameObject::setSize(float newdimX, float newdimY) {
	dim = { newdimX, newdimY };
	center = { this->dim.x / 2, this->dim.y / 2 };
}

void gameObject::gravity() {
	//TODO add collision detection and gravity system
    acc = { acc.x, (acc.y + baseGravity) };
    //Call collision detection here too?
}
bool gameObject::checkColission(gameObject currentObject){
    //TODO make collision checks
    if (enableCollision) {
        //Code here
        
    }
    return false;
}
void gameObject::move(float deltaTime) {
	acc = glm::normalize(acc);
	acc += acc * deltaTime * vel;
}

void gameObject::update(float deltaTime, gl2d::Renderer2D& renderer) {
	if (enableGravity) { this->gravity(); }
	if (acc != glm::vec2{0, 0}) { this->move(deltaTime); this->pos += acc; }

	if (currentTextureType == normal) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), dim }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f);
	} else if (currentTextureType == atlas) {
		renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), dim }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f, objectAtlas.get(currentAtlasPoint.x, currentAtlasPoint.y));
	}
}

