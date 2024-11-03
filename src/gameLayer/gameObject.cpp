#include "gameObject.h"
#include <gl2d/gl2d.h>
#include <iostream>



int objectCount = 0;



void gameObject::createObject(objectType type, const char* textureFile) {
	objectCount++;
	id = objectCount;
	pos.x, pos.y = 100;
	size = { 100, 100 };
	center = { size.x / 2, size.y / 2 };
	rotation = 0;

	objectTexture.loadFromFile(textureFile, true);

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


void gameObject::update(float deltaTime, gl2d::Renderer2D& renderer) {
	this->gravity();



	renderer.renderRectangle({ (pos.x - center.x), (pos.y - center.y), size }, objectTexture,
		Colors_White, {}, glm::degrees(rotation) + 90.f);

}





