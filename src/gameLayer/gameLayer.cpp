#include "glm/geometric.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <cstdio>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>
#include <gameObject.hpp>


//Save data
struct playerData
{
    glm::vec2 shipSize = { 250.f, 250.f };
    glm::vec2 vel = { 500.0f, 500.0f };
    float cameraSize = 0.4f;
    bool mouseShowing = true;
    std::vector<gameObject> bullets;
    std::vector<gameObject> enemies;
} playData;


//Declarations
gl2d::Renderer2D renderer;
float fpsCurrentTimer;
float fps;
const int BGs = 4;
gl2d::Texture backGroundTexture[BGs];
TiledRenderer tiledRenderer[BGs];
gameObject player;








void printFpsCounter(float deltaTime) {
	fpsCurrentTimer += deltaTime;
	fps++;
	if (fpsCurrentTimer >= 1) {
        //gameObject::printObjectState(&player);
		fpsCurrentTimer = 0;
		std::cout << fps << " ";
		fps = 0;
	}
}
void playerMove(float DT) {
	if (platform::isButtonHeld(platform::Button::W) || platform::isButtonHeld(platform::Button::Up)) {
		player.setAcc(player.getAcc().x, -1);
	}
	if (platform::isButtonHeld(platform::Button::S) || platform::isButtonHeld(platform::Button::Down)) {
		player.setAcc(player.getAcc().x, 1);
	}
	if (platform::isButtonHeld(platform::Button::A) || platform::isButtonHeld(platform::Button::Left)) {
		player.setAcc(-1, player.getAcc().y);
	}
	if (platform::isButtonHeld(platform::Button::D) || platform::isButtonHeld(platform::Button::Right)) {
		player.setAcc(1, player.getAcc().y);
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
        gameObject b(gameObject::objectType::bullet, RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png", 
                        gameObject::atlas, { 3,2 }, { 1,0 }, 500);
		b.setPos(player.getPos().x, player.getPos().y);
		b.setRotation(player.getRotation());
		b.setVel(1500, 1500);
		b.setAcc(mouseDirection.x, mouseDirection.y);
		b.setDim(50, 50);
        gameObject::addToLayer(&b, "bullets");

		playData.bullets.push_back(b);
	}
}
void enemyacc(float DT, gameObject &e) {
	glm::vec2 directionToPlayer = player.getPos() - e.getPos();
	
	if (glm::length(directionToPlayer) == 0) { directionToPlayer = { 1,0 }; }
	else { directionToPlayer = glm::normalize(directionToPlayer); }
	
	glm::vec2 newDirection = {};
	if (glm::length(directionToPlayer + e.getEnemyViewDirection()) <= 0.2f){ 
		newDirection = glm::vec2(directionToPlayer.y, -directionToPlayer.x);
	} else{ newDirection = DT * e.getTurningSpeed() * directionToPlayer + e.getEnemyViewDirection(); }
	
	e.setEnemyViewDirection(glm::normalize(newDirection));
	e.setPos(e.getPos().x + e.getEnemyViewDirection().x * DT * e.getVel().x, e.getPos().y + e.getEnemyViewDirection().y * DT * e.getVel().y);
}
void spawnEnemy(float DT) {
	glm::uvec2 enemyTypes[] = { {0,0}, {0,1}, {2,0}, {3, 1} };
	gameObject e(gameObject::entity, RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png",
		            gameObject::atlas, { 5,2 }, enemyTypes[rand() % 4], 128);
	e.setPos(player.getPos().x, player.getPos().y);
	e.setDim(playData.shipSize.x, playData.shipSize.y);
    float newVel = 400 + rand() % 1000;
    e.setVel(newVel, newVel);
	e.setTurningSpeed(2.f + (rand() & 1000) / 500.0f);
    gameObject::addToLayer(&e, "enemy");
    e.setCollisionBool(true);
	playData.enemies.push_back(e);
}


bool initGame() {
	//Initializing stuff for the renderer
	gl2d::init();
	renderer.create();
	std::srand(std::time(0));
    gameObject::newLayer("default", 10);
    gameObject::newLayer("enemy", 11);
    gameObject::newLayer("player", 15);
    gameObject::newLayer("bullets", 8);

    //Player creation
    player = gameObject(gameObject::entity, RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", gameObject::atlas, { 5,2 }, { 1,0 }, 128);
	player.setDim(playData.shipSize.x, playData.shipSize.y);
    gameObject::addToLayer(&player, "player");
	player.setVel(playData.vel.x, playData.vel.y);
    player.setCollisionBool(true);

    {//Background texture init
        backGroundTexture[0].loadFromFile(RESOURCES_PATH "background1.png", true, true);
        backGroundTexture[1].loadFromFile(RESOURCES_PATH "background2.png", true, true);
        backGroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true, true);
        backGroundTexture[3].loadFromFile(RESOURCES_PATH "background4.png", true, true);

        tiledRenderer[0].texture = backGroundTexture[0];
        tiledRenderer[1].texture = backGroundTexture[1];
        tiledRenderer[2].texture = backGroundTexture[2];
        tiledRenderer[3].texture = backGroundTexture[3];
        tiledRenderer[0].paralaxStrength = 0.f;
        tiledRenderer[1].paralaxStrength = 0.2f;
        tiledRenderer[2].paralaxStrength = 0.4f;
        tiledRenderer[3].paralaxStrength = 0.7f;
    }

	return true;
}



bool gameLogic(float deltaTime) {
    //Init after the flush
    int w = 0; int h = 0;
    w = platform::getFrameBufferSizeX(); //Window w
    h = platform::getFrameBufferSizeY(); //Window h
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT); //Clear screen
    renderer.updateWindowMetrics(w, h);
    


    //Mouse direction checking
    glm::vec2 mousePos = platform::getRelMousePosition();
    glm::vec2 screenCenter(player.getPos() - renderer.currentCamera.position);   
    glm::vec2 mouseDirection = mousePos - screenCenter;
    if (glm::length(mouseDirection) == 0.f) {
        mouseDirection = { 1,0 };
    } else {
        mouseDirection = normalize(mouseDirection);
    }
    player.setRotation(atan2(mouseDirection.y, -mouseDirection.x));
    platform::showMouse(playData.mouseShowing);


	//Camera setup
	renderer.currentCamera.zoom = playData.cameraSize;
	renderer.currentCamera.follow(player.getPos(), deltaTime * 450, 10, 50, w, h);

	
	//Update entities and key checks
	playerMove(deltaTime);
	cameraSizeChange(deltaTime);
	bulletShooting(deltaTime, mouseDirection);
	for (auto& e : playData.enemies) { enemyacc(deltaTime, e); }


	//Rendering everything
    for (int i = 0; i < BGs; i++) { tiledRenderer[i].render(renderer); }
	for (int i = 0; i < playData.bullets.size(); i++) {
		if (glm::distance(playData.bullets[i].getPos(), player.getPos()) > 5000) { 
			playData.bullets.erase(playData.bullets.begin() + i); i--; continue; 
		}
	}
    gameObject::updateAll(deltaTime, renderer);


	//Reset player acc
	player.setAcc(0, 0);

    {//ImGui debug Ui
        ImGui::Begin("debug");
        ImGui::Text("Bullets count: %d \n", (int)playData.bullets.size());
        ImGui::Text("enemy count: %d \n", (int)playData.enemies.size());
        ImGui::Text("object count: %d \n", (int)gameObject::getObjectCount());
        ImGui::Text("fps: %d \n", (int)(std::floor(1 / deltaTime)));
        if (ImGui::Button("spawn enemy")) { spawnEnemy(deltaTime); }
        ImGui::End();
    }

	renderer.flush();
	return true;
}

//This function might not be called if the program is forced closed
void closeGame() {



}

