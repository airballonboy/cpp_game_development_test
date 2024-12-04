#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>
#include <gameObject.hpp>


struct playerData
{
	glm::vec2 shipSize = { 250.f, 250.f };
	float speed = 500.0f;
	float cameraSize = 0.4f;
	bool mouseShowing = true;
	std::vector<gameObject> bullets;
	std::vector<gameObject> enemies;
} playData;

#pragma region declerations
gl2d::Renderer2D renderer;
float fpsCurrentTimer;
float fps;
const int BGs = 4;
gl2d::Texture backGroundTexture[BGs];
TiledRenderer tiledRenderer[BGs];
gameObject player;
#pragma endregion








void printFpsCounter(float deltaTime) {
	fpsCurrentTimer += deltaTime;
	fps++;
	if (fpsCurrentTimer >= 1) {
		fpsCurrentTimer = 0;
		std::cout << fps << " ";
		fps = 0;
	}
}
void playerMove(float DT) {
	if (platform::isButtonHeld(platform::Button::W) || platform::isButtonHeld(platform::Button::Up)) {
		player.movement.y = -1;
	}
	if (platform::isButtonHeld(platform::Button::S) || platform::isButtonHeld(platform::Button::Down)) {
		player.movement.y = 1;
	}
	if (platform::isButtonHeld(platform::Button::A) || platform::isButtonHeld(platform::Button::Left)) {
		player.movement.x = -1;
	}
	if (platform::isButtonHeld(platform::Button::D) || platform::isButtonHeld(platform::Button::Right)) {
		player.movement.x = 1;
	}
}
void cameraSizeChange(float DT) {
	if (platform::isButtonHeld(platform::Button::I) && playData.cameraSize < 0.7f) {
		playData.cameraSize += (DT / 50);
	} else if (platform::isButtonHeld(platform::Button::K) && playData.cameraSize > 0.3f) {
		playData.cameraSize -= (DT / 50);
	}
	if (platform::isButtonHeld(platform::Button::LeftCtrl) && platform::isButtonPressedOn(platform::Button::Enter)) {
		if (platform::isFullScreen()) {
			platform::setFullScreen(false);
		}else { platform::setFullScreen(true); }
	}
}
void bulletShooting(float DT, glm::vec2 mouseDirection) {
	if (platform::isLMousePressed()) {
		gameObject b;
		b.pos = player.pos;
		b.rotation = player.rotation;
		b.speed = 1500;
		b.movement = mouseDirection;
		b.setSize(50, 50);
		b.createObject(gameObject::objectType::bullet, RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png",
                 gameObject::atlas, { 3,2 }, { 1,0 }, 500);

		playData.bullets.push_back(b);
	}
}
void enemyMovement(float DT, gameObject &e) {
	glm::vec2 directionToPlayer = player.pos - e.pos;
	if (glm::length(directionToPlayer) == 0) { directionToPlayer = { 1,0 }; } else { directionToPlayer = glm::normalize(directionToPlayer); }
	glm::vec2 newDirection = {};
	if (glm::length(directionToPlayer + e.enemyViewDirection) <= 0.2f){ newDirection = glm::vec2(directionToPlayer.y, -directionToPlayer.x); } 
	else{ newDirection = DT * e.turningSpeed * directionToPlayer + e.enemyViewDirection; }
	e.enemyViewDirection = glm::normalize(newDirection);
	e.pos += e.enemyViewDirection * DT * e.speed;
}
void spawnEnemy(float DT) {
	glm::uvec2 enemyTypes[] = { {0,0}, {0,1}, {2,0}, {3, 1} };
	gameObject e;
	e.createObject(gameObject::entity, RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", gameObject::atlas, { 5,2 }, enemyTypes[rand() % 4], 128);
	e.pos = player.pos;
	e.setSize(playData.shipSize.x, playData.shipSize.y);
	e.speed = 400 + rand() % 1000;
	e.turningSpeed = 2.f + (rand() & 1000) / 500.0f;
	playData.enemies.push_back(e);
}


bool initGame() {
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();
	std::srand(std::time(0));
  
	player.createObject(gameObject::entity, RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png",
                     gameObject::atlas, { 5,2 }, { 1,0 }, 128);
	player.setSize(playData.shipSize.x, playData.shipSize.y);
	player.speed = playData.speed;

  gl2d::Texture i;

#pragma region background texture loading
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
	glm::vec2 screenCenter(player.pos - renderer.currentCamera.position);
	glm::vec2 mouseDirection = mousePos - screenCenter;

	if (glm::length(mouseDirection) == 0.f) {
		mouseDirection = { 1,0 };
	} else {
		mouseDirection = normalize(mouseDirection);
	}

	player.rotation = atan2(mouseDirection.y, -mouseDirection.x);
	platform::showMouse(playData.mouseShowing);

#pragma endregion

#pragma region camera settings

	renderer.currentCamera.zoom = playData.cameraSize;
	renderer.currentCamera.follow(player.pos, deltaTime * 450, 10, 50, w, h);

#pragma endregion

#pragma region input checking

	playerMove(deltaTime);
	cameraSizeChange(deltaTime);
	bulletShooting(deltaTime, mouseDirection);
	for (auto& e : playData.enemies) { enemyMovement(deltaTime, e); }

#pragma endregion

#pragma region rendering

	for (int i = 0; i < BGs; i++) { tiledRenderer[i].render(renderer); }
	for (int i = 0; i < playData.bullets.size(); i++) {
		if (glm::distance(playData.bullets[i].pos, player.pos) > 5000) { 
      playData.bullets.erase(playData.bullets.begin() + i); i--; 
      continue; 
    }
		playData.bullets[i].update(deltaTime, renderer);
	}
	for (auto& e : playData.enemies) { e.update(deltaTime, renderer); }


	player.update(deltaTime, renderer);

#pragma endregion

	//printFpsCounter(deltaTime);

	player.movement = { 0, 0 };


#pragma region ImGui
	ImGui::Begin("debug");

	ImGui::Text("Bullets count: %d \n", (int)playData.bullets.size());
	ImGui::Text("enemy count: %d \n", (int)playData.enemies.size());
	ImGui::Text("object count: %d \n", (int)gameObject::getObjectCount());
	ImGui::Text("fps: %d \n", (int)(std::round(1 / deltaTime)));
	if (ImGui::Button("spawn enemy")) { spawnEnemy(deltaTime); }
	
	ImGui::End();
#pragma endregion
	
  printFpsCounter(deltaTime);

	renderer.flush();
	return true;
}

//This function might not be be called if the program is forced closed
void closeGame() {



}

