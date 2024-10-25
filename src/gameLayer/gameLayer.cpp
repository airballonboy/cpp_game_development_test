#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>

struct playerData
{
	glm::vec2 playerPos = { 100, 100 };
	float spaceShipAngle = 0.f;
	glm::vec2 shipSize = { 250.f, 250.f };
	glm::vec2 shipCenter = { shipSize.x / 2, shipSize.y / 2 };
	float speed = 500.0f;
	float cameraSize = 0.4f;
	bool mouseShowing = true;
} playData;

#pragma region declerations
gl2d::Renderer2D renderer;
gl2d::Texture spaceShipTexture;
constexpr int BGs = 4;
gl2d::Texture backGroundTexture[BGs];
TiledRenderer tiledRenderer[BGs];
#pragma endregion



void playerMove(float DT) {
	glm::vec2 move = {};

	if (platform::isButtonHeld(platform::Button::W) || platform::isButtonHeld(platform::Button::Up)) {
		move.y = -1;
	}
	if (platform::isButtonHeld(platform::Button::S) || platform::isButtonHeld(platform::Button::Down)) {
		move.y = 1;
	}
	if (platform::isButtonHeld(platform::Button::A) || platform::isButtonHeld(platform::Button::Left)) {
		move.x = -1;
	}
	if (platform::isButtonHeld(platform::Button::D) || platform::isButtonHeld(platform::Button::Right)) {
		move.x = 1;
	}

	if (move.x != 0 || move.y != 0) {
		move = glm::normalize(move);
		move *= DT * playData.speed;
		playData.playerPos += move;
	}
}
void cameraSizeChange(float DT) {
	if (platform::isButtonHeld(platform::Button::I) && playData.cameraSize < 0.7f) {
		playData.cameraSize += (DT / 50);
	}else if (platform::isButtonHeld(platform::Button::K) && playData.cameraSize > 0.3f) {
		playData.cameraSize -= (DT / 50);
	}
}



bool initGame() {
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();


#pragma region texture loading

	spaceShipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/blue.png", true);
	backGroundTexture[0].loadFromFile(RESOURCES_PATH "background1.png", true);
	backGroundTexture[1].loadFromFile(RESOURCES_PATH "background2.png", true);
	backGroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true);
	backGroundTexture[3].loadFromFile(RESOURCES_PATH "background4.png", true);



	tiledRenderer[0].texture = backGroundTexture[0];
	tiledRenderer[1].texture = backGroundTexture[1];
	tiledRenderer[2].texture = backGroundTexture[2];
	tiledRenderer[3].texture = backGroundTexture[3];
	tiledRenderer[0].paralaxStrength = 0;
	tiledRenderer[1].paralaxStrength = 0.2;
	tiledRenderer[2].paralaxStrength = 0.4;
	tiledRenderer[3].paralaxStrength = 0.7;



#pragma endregion 
	
	return true;
}



bool gameLogic(float deltaTime) {
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion

#pragma region mouse pos

	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);
	glm::vec2 mouseDirection = mousePos - screenCenter;

	if (glm::length(mouseDirection) == 0.f) {
		mouseDirection = { 1,0 };
	} else {
		mouseDirection = normalize(mouseDirection);
	}

	playData.spaceShipAngle = atan2(mouseDirection.y, -mouseDirection.x);
	platform::showMouse(playData.mouseShowing);

#pragma endregion

#pragma region camera settings

	renderer.currentCamera.zoom = playData.cameraSize;
	renderer.currentCamera.follow(playData.playerPos, deltaTime * 450, 10, 50, w, h);

#pragma endregion

#pragma region input checking
	if (platform::isButtonHeld) {
		playerMove(deltaTime);
		cameraSizeChange(deltaTime);
	}
#pragma endregion
	
#pragma region rendering

	for (int i = 0; i < BGs; i++) { tiledRenderer[i].render(renderer); }

	renderer.renderRectangle({ (playData.playerPos.x - playData.shipCenter.x), (playData.playerPos.y - playData.shipCenter.y),
		playData.shipSize.x, playData.shipSize.y }, spaceShipTexture, Colors_White, {},
		glm::degrees(playData.spaceShipAngle) + 90.f);
	
#pragma endregion


	renderer.flush();
	return true;
}

//This function might not be be called if the program is forced closed
void closeGame() {



}

