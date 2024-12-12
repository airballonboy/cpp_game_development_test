#pragma once
#include <gl2d/gl2d.h>
#include <vector>



class gameObject {
private:

	int32_t id;

	gl2d::Texture objectTexture;
	gl2d::TextureAtlasPadding objectAtlas;

public:
	enum objectType { bullet, background, entity, staticEntity };
	enum textureType { normal, atlas };
	textureType currentTextureType;
	objectType currentType;
	void createObject(objectType, const char*, textureType = normal, glm::vec2 = { 1, 1 }, glm::vec2 = { 0, 0 }, int = 128);
	void update(float, gl2d::Renderer2D&);
	void gravity();
    bool checkColission(gameObject);
	void setSize(float, float);
	static int getObjectCount();
	int getId();
	bool isTheSameObject(gameObject);
	void move(float);
    float baseGravity = 9.8;
	float rotation = 0;
	float turningSpeed = 1;
	glm::vec2 acc = { 0, 0 };//Acceleration
    glm::vec2 vel = { 0, 0 };//Velocity
	glm::vec2 pos = { 100, 100 };//Position
	glm::vec2 dim = { 100, 100 };//Dimensions
	glm::vec2 center = { dim.x / 2, dim.y / 2 };
	glm::vec2 currentAtlasPoint;
	glm::vec2 enemyViewDirection = { 1,0 };
	bool enableGravity = false;
    bool enableCollision = false;




};
class loadOnceClass {
public:
	std::vector<const char*> loadedTexturesNames;
	std::vector<gl2d::Texture> loadedTextures;
	std::vector<gl2d::TextureAtlasPadding> loadedTextureAtlases;

	int checkTextures(const char*, bool, int = 128, glm::vec2 = { 0, 0 });

};
