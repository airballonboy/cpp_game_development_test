#pragma once
#include <gl2d/gl2d.h>



class gameObject {
private:

	int id;

	gl2d::Texture objectTexture;
	gl2d::TextureAtlasPadding objectAtlas;

public:
	enum objectType { bullet, background, entity, staticEntity };
	enum textureType { normal , atlas };
	textureType currentTextureType;
	objectType currentType;
	void createObject(objectType, const char*, textureType = normal, glm::vec2 = { 1, 1 }, glm::vec2 = { 0, 0 }, int = 128);
	void createObject(objectType, textureType = normal, glm::vec2 = { 1, 1 }, glm::vec2 = { 0, 0 }, int = 128);
	void update(float, gl2d::Renderer2D&);
	void gravity();
	void setSize(float, float);
	int getId();
	bool isTheSameObject(gameObject);
	void move(float);
	glm::vec2 movement = { 0, 0 };
	float rotation = 0;
	glm::vec2 pos = { 100, 100 };
	glm::vec2 velocity;
	glm::vec2 size = { 100, 100 };
	glm::vec2 center = {size.x / 2, size.y / 2};
	glm::vec2 currentAtlasPoint;
	float speed = 1;
	bool enableGravity;





};
class loadOnceClass {
public:
	void loadBullets(gameObject::objectType, const char*, gameObject::textureType = gameObject::normal, glm::vec2 = { 1, 1 }, glm::vec2 = { 0, 0 }, int = 128);
};
