#pragma once

class gameObject{			
private:

	int objectCount = 0;

public:
	enum objectType{background, entity, staticEntity};
	void createObject(objectType, const char*);
	




};
