#pragma once
#include <gl2d/gl2d.h>


class gameObject{			
private:

	int objectCount = 0;
	gl2d::Texture objectTexture;
	//gl2d::Renderer2D currentRenderer;

public:
	enum objectType{background, entity, staticEntity};
	void createObject(objectType, const char*);
	void update(float, gl2d::Renderer2D& renderer);
	void setSize(float, float);
	float rotation;
	glm::vec2 pos;
	glm::vec2 size;
	float sizeX, sizeY;
	glm::vec2 center;




};
