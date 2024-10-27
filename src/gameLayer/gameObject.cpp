#include "gameObject.h"
#include <gl2d/gl2d.h>
#include <iostream>


//gl2d::Renderer2D renderer;


void gameObject::createObject(objectType type, const char* textureFile) {
	gameObject::objectCount++;
	int id = gameObject::objectCount;
	std::cout << textureFile;

	objectTexture.loadFromFile(textureFile, true);
}
void gameObject::update(float deltaTime) {
	//renderer.renderRectangle({ (playData.playerPos.x - playData.shipCenter.x), (playData.playerPos.y - playData.shipCenter.y),
	//playData.shipSize.x, playData.shipSize.y }, objectTexture, Colors_White, {},
	//	glm::degrees(playData.spaceShipAngle) + 90.f);
}
