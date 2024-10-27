#pragma once
#include <gl2d/gl2d.h>


class gameObject{			
private:

	int objectCount = 0;
	gl2d::Texture objectTexture;


public:
	enum objectType{background, entity, staticEntity};
	void createObject(objectType, const char*);
	void update(float);





};
