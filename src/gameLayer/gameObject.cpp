#include "gameObject.hpp"
#include <string>
#include <gl2d/gl2d.h>
#include <algorithm>
#include <ostream>
#include <vector>
#include <iostream>



int objectCount = 0;
loadOnceClass loadOnce;
// Definition of static member
std::vector<gameObject> gameObject::gameObjects;
std::vector<gameObject::renderLayer> gameObject::layer;


//Checks if the current texture is loaded or not and loads it if not
int loadOnceClass::checkTextures(const char* Texture, bool atlas, bool minPixelated, bool magPixelated, bool MipMap, 
                                 int atlasdim, glm::vec2 atlasPoint) {
	
    for (const char* T : loadedTexturesNames) {
		if (Texture == T) { return std::distance(loadedTexturesNames.begin() 
			,std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture)); }
	}
	loadOnceClass::loadedTexturesNames.push_back(Texture);
	gl2d::Texture t;
	if (!atlas) {
		t.loadFromFile(Texture, minPixelated, magPixelated, MipMap);
	} else {
		gl2d::TextureAtlasPadding tPadding;
		t.loadFromFileWithPixelPadding(Texture, atlasdim, minPixelated, magPixelated, MipMap);
		tPadding = gl2d::TextureAtlasPadding(atlasPoint.x, atlasPoint.y, t.GetSize().x, t.GetSize().y);
		loadOnceClass::loadedTextureAtlases.push_back(tPadding);
	}
	loadOnceClass::loadedTextures.push_back(t);
	return std::distance(loadedTexturesNames.begin(), std::find(loadedTexturesNames.begin(), loadedTexturesNames.end(), Texture));
}
void gameObject::newLayer(std::string name, int order){
    renderLayer L;
    L.name = name;
    L.order = order;
    layer.push_back(L);
    std::sort(layer.begin(), layer.end(), [](const renderLayer& a, const renderLayer& b){
        return a.order < b.order;
    });
}
void gameObject::addToLayer(gameObject* GO, std::string name){
    auto check = [](bool C, std::string thisName, std::vector<renderLayer>* L)
        {for (renderLayer RL : *L) {if (RL.name == thisName){C = true;break;} C = false;}return C;};
    
    if (!check (true, name, &layer)){
        std::cerr << "name doesn't match any layer";
    }
    GO->currentLayer = name;
    gameObjects[GO->id - 1].currentLayer = name;
}
int gameObject::getObjectCount() {
	return objectCount;
}

//Creating the object and loading it's texture if it isn't loaded
gameObject::gameObject(objectType _type, const char* _textureFile, textureType _currentTextureType
                            , glm::vec2 _atlasPoint, glm::vec2 _texturePoint, int _atlasdim) {

	objectCount++;
	id = objectCount;
	currentTextureCoords = _texturePoint;
	currentTextureType = _currentTextureType;
	currentType = _type;
	if (currentTextureType == normal) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(_textureFile, false, false ,true, true)];
	} else if (currentTextureType == atlas) {
		objectTexture = loadOnce.loadedTextures[loadOnce.checkTextures(_textureFile, true, false, true, true, _atlasdim, _atlasPoint)];
		objectAtlas = loadOnce.loadedTextureAtlases[loadOnce.checkTextures(_textureFile, true, false, true, true, _atlasdim, _atlasPoint)];
	}
    gameObjects.emplace_back(*this);
    addToLayer(&gameObjects[this->id - 1], currentLayer);
}
gameObject::gameObject(){
    
}

//Checks if two objects are the same
bool gameObject::isTheSameObject(gameObject otherObject) {
	if (this->id == otherObject.id) {
		return true;
	} else { return false; }
}

bool gameObject::checkColission(gameObject currentObject){
    //TODO make collision checks
    if (enableCollision) {
        //Code here
        
    }
    return false;
}
void gameObject::move(float deltaTime) {
    glm::vec2 normalizedAcc = glm::normalize(acc);
	this->setAcc(normalizedAcc.x + (normalizedAcc.x * deltaTime * vel.x), normalizedAcc.y + (normalizedAcc.y * deltaTime * vel.y));
}

void gameObject::gravity() {
	//TODO add collision detection and gravity system
    this->setAcc(acc.x, (acc.y + baseGravity));
    //Call collision detection here too?
}

void gameObject::updateAll(float deltaTime, gl2d::Renderer2D& renderer) {
    //NOTE still doesn't work but better
    for(int i = 0; i < layer.size(); i++) {
        for (auto& go : gameObjects){
            if ((go.currentLayer == layer[i].name) && !go.rendered) {
                updateByRef(deltaTime, renderer, &gameObjects[go.id - 1]);
                gameObjects[go.id - 1].rendered = true;
            }
        }
    }
    for(auto& go : gameObjects){ gameObjects[go.id - 1].rendered = false; }
}
void gameObject::updateByRef(float deltaTime, gl2d::Renderer2D& renderer, gameObject* that){
	if (that->enableGravity) { that->gravity(); }
	if (that->acc != glm::vec2{0, 0}) { that->move(deltaTime); that->setPos(that->pos.x + that->acc.x, that->pos.y + that->acc.y); }
	if (that->currentTextureType == gameObject::normal) {
		renderer.renderRectangle({ (that->pos.x - that->pivot.x), (that->pos.y - that->pivot.y), that->dim }, that->objectTexture,
			Colors_White, {}, glm::degrees(that->rotation) + 90.f);
	} else if (that->currentTextureType == gameObject::atlas) {
		renderer.renderRectangle({ (that->pos.x - that->pivot.x), (that->pos.y - that->pivot.y), that->dim }, that->objectTexture, Colors_White,
            {},glm::degrees(that->rotation) + 90.f, that->objectAtlas.get(that->currentTextureCoords.x, that->currentTextureCoords.y));
	}
}
void gameObject::printObjectState(gameObject* objectPtr){
    gameObject o = *objectPtr;
    std::cout << "id: " << o.id<< std::endl;
    std::cout << "currentTextureType: " << o.currentTextureType<< std::endl;
    std::cout << "currentType: " << o.currentType<< std::endl;
    std::cout << "currentSortingLayer: " << o.currentLayer<< std::endl;
    std::cout << "baseGravity: " << o.baseGravity<< std::endl;
    std::cout << "rotation: " << o.rotation<< std::endl;
    std::cout << "turningSpeed: " << o.turningSpeed<< std::endl;
    std::cout << "acc.x: " << o.acc.x<< std::endl;
    std::cout << "acc.y: " << o.acc.y<< std::endl;
    std::cout << "vel.x: " << o.vel.x<< std::endl;
    std::cout << "vel.y: " << o.vel.y<< std::endl;
    std::cout << "pos.x: " << o.pos.x<< std::endl;
    std::cout << "pos.y: " << o.pos.y<< std::endl;
    std::cout << "dim.x: " << o.dim.x<< std::endl;
    std::cout << "dim.y: " << o.dim.y<< std::endl;
    std::cout << "pivot.x: " << o.pivot.x<< std::endl;
    std::cout << "pivot.y: " << o.pivot.y<< std::endl;
    std::cout << "currentTextureCoords.x: " << o.currentTextureCoords.x<< std::endl;
    std::cout << "currentTextureCoords.y: " << o.currentTextureCoords.y<< std::endl;
    std::cout << "enableGravity: " << o.enableGravity<< std::endl;
    std::cout << "enableCollision: " << o.enableCollision<< std::endl;


    std::cout << "---------------------------------------------------" << std::endl;
}
void gameObject::update(float deltaTime, gl2d::Renderer2D& renderer) {
	if (enableGravity) { this->gravity(); }
	if (acc != glm::vec2{0, 0}) { this->move(deltaTime); this->setPos(this->getPos().x+this->getAcc().x, this->getPos().y+this->getAcc().y); }
	if (currentTextureType == normal) {
		renderer.renderRectangle({ (pos.x - pivot.x), (pos.y - pivot.y), dim }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f);
	} else if (currentTextureType == atlas) {
		renderer.renderRectangle({ (pos.x - pivot.x), (pos.y - pivot.y), dim }, objectTexture,
			Colors_White, {}, glm::degrees(this->rotation) + 90.f, objectAtlas.get(currentTextureCoords.x, currentTextureCoords.y));
	}
}







//Setters
void gameObject::setDim(float newDimX, float newDimY) {
	this->dim = { newDimX, newDimY };
	this->pivot = { this->dim.x / 2, this->dim.y / 2 };
    gameObjects[(this->id - 1)].dim = this->dim;
    gameObjects[(this->id - 1)].pivot = this->pivot;
}
void gameObject::setAcc(float newAccX, float newAccY){
    this->acc = { newAccX, newAccY };
    gameObjects[(this->id - 1)].acc = this->acc;
}
void gameObject::setVel(float newVelX, float newVelY){
    this->vel = { newVelX, newVelY };
    gameObjects[(this->id - 1)].vel = this->vel;
}
void gameObject::setPos(float newPosX, float newPosY){
    this->pos = { newPosX, newPosY };
    gameObjects[(this->id - 1)].pos = this->pos;
}
void gameObject::setPivot(float newPivotX, float newPivotY){
    this->pivot = { newPivotX, newPivotY };
    gameObjects[(this->id - 1)].pivot = this->pivot;
}
void gameObject::setEnemyViewDirection(glm::vec2 newDir){
    this->enemyViewDirection = newDir;
    gameObjects[(this->id - 1)].enemyViewDirection = this->enemyViewDirection;
}
void gameObject::setGravityBool(bool newGravityBool){
    this->enableGravity = newGravityBool;
    gameObjects[(this->id - 1)].enableGravity = this->enableGravity;
}
void gameObject::setCollisionBool(bool newCollisionBool){
    this->enableCollision = newCollisionBool;
    gameObjects[(this->id - 1)].enableCollision = this->enableCollision;
}
void gameObject::setBaseGravity(float newBaseGravity){
    this->baseGravity = newBaseGravity;
    gameObjects[(this->id - 1)].baseGravity = this->baseGravity;
}
void gameObject::setRotation(float newRotation){
    this->rotation = newRotation;
    gameObjects[(this->id - 1)].rotation = this->rotation;
}
void gameObject::setTurningSpeed(float newTurningSpeed){
    this->turningSpeed = newTurningSpeed;
    gameObjects[(this->id - 1)].turningSpeed = this->turningSpeed;
}




//Getters
glm::vec2 gameObject::getDim() {
    return gameObjects[(this->id - 1)].dim;
}
glm::vec2 gameObject::getAcc(){
    return gameObjects[(this->id - 1)].acc;
}
glm::vec2 gameObject::getVel(){
    return gameObjects[(this->id - 1)].vel;
}
glm::vec2 gameObject::getPos(){
    return gameObjects[(this->id - 1)].pos;
}
glm::vec2 gameObject::getPivot(){
    return gameObjects[(this->id - 1)].pivot;
}
glm::vec2 gameObject::getEnemyViewDirection(){
    return gameObjects[(this->id - 1)].enemyViewDirection;
}
bool gameObject::getGravityBool(){
    return gameObjects[(this->id - 1)].enableGravity;
}
bool gameObject::getCollisionBool(){
    return gameObjects[(this->id - 1)].enableCollision;
}
float gameObject::getBaseGravity(){
    return gameObjects[(this->id - 1)].baseGravity;
}
float gameObject::getRotation(){
    return gameObjects[(this->id - 1)].rotation;
}
float gameObject::getTurningSpeed(){
    return gameObjects[(this->id - 1)].turningSpeed;
}
int gameObject::getId() {
	return this->id;
}




