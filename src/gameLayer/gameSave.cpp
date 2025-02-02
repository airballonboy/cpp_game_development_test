#include "gameSave.hpp"
#include "gameObject.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using json = nlohmann::json;

void save::saveGameObjects(const std::string &filename) {
	json j;
	for (auto& obj : gameObject::gameObjects) {
		j["gameObjects"].push_back({
			{"id: ", obj.getId()},
			{"pos: ", {obj.getPos().x, obj.getPos().y}},
			{"vel: ", {obj.getVel().x, obj.getVel().y}},
			{"dim: ", {obj.getDim().x, obj.getDim().y}},
			{"acc: ", {obj.getAcc().x, obj.getAcc().y}},
			{"textureCTX: ", {
				obj.getTextureCTX().Texture, obj.getTextureCTX().atlas,
				obj.getTextureCTX().minPixelated, obj.getTextureCTX().magPixelated,
				obj.getTextureCTX().MipMap, obj.getTextureCTX().atlasdim,
				obj.getTextureCTX().atlasPoint.x, obj.getTextureCTX().atlasPoint.y 
			}},
			{"rotation: ", obj.getRotation()},
			{"gravityBool: ", obj.getGravityBool()},
			{"collisionBool: ", obj.getCollisionBool()},
		});
	}
	remove(filename.c_str());
	std::ofstream file(filename);
	file << j.dump(4);
	file.close();
}



void save::loadGameObjects(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    json j;
    file >> j;
    file.close();

    gameObject::gameObjects.clear();
    for (auto& objJson : j["gameObjects"]) {
        auto& tCTX = objJson["textureCTX: "];
        std::cout << 0;
        
        gameObject obj;
        std::cout << 120 << std::endl;
        std::cout << tCTX[0].dump() << std::endl;  // Debugging output

        std::string textureStr = tCTX[0];
		const char* textureCStr = textureStr.c_str();
        
        if (tCTX.size() >= 8 && tCTX[1]) {
            obj = gameObject(gameObject::entity, textureCStr, gameObject::atlas, {tCTX[6], tCTX[7]}, {0,0}, 128);
        } else {
            obj = gameObject(gameObject::entity, textureCStr, gameObject::normal);
        }
        
        obj.setPos(objJson["pos: "][0], objJson["pos: "][1]);
        obj.setVel(objJson["vel: "][0], objJson["vel: "][1]);
        obj.setDim(objJson["dim: "][0], objJson["dim: "][1]);
        obj.setAcc(objJson["acc: "][0], objJson["acc: "][1]);
        obj.setTextureCTX({textureStr.c_str(), tCTX[1], tCTX[2], tCTX[3], tCTX[4], tCTX[5], {tCTX[6], tCTX[7]}});
        obj.setRotation(objJson["rotation: "]);
        obj.setGravityBool(objJson["gravityBool: "]);
        obj.setCollisionBool(objJson["collisionBool: "]);
    }
}








/*void save::loadGameObjects(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    json j;
    file >> j;
    file.close();

    gameObject::gameObjects.clear();
    for (auto& objJson : j["gameObjects"]) {
		auto& tCTX = objJson["textureCTX: "];
		gameObject obj;
		std::string tCTXTexture = tCTX[0].dump();

		if (tCTX[1]) obj = gameObject(gameObject::entity, tCTXTexture.c_str(), gameObject::atlas, {tCTX[6], tCTX[7]}, {0,0}, 128);	
		else  obj = gameObject(gameObject::entity, tCTXTexture.c_str(), gameObject::normal);	
		obj.setPos(objJson["pos: "][0], objJson["pos: "][1]);
        obj.setVel(objJson["vel: "][0], objJson["vel: "][1]);
        obj.setDim(objJson["dim: "][0], objJson["dim: "][1]);
		obj.setAcc(objJson["acc: "][0], objJson["acc: "][1]);
		obj.setTextureCTX({tCTX[0].dump().c_str(),tCTX[1],tCTX[2],tCTX[3],tCTX[4],tCTX[5],{tCTX[6],tCTX[7]}});
        obj.setRotation(objJson["rotation: "]);
        obj.setGravityBool(objJson["gravityBool: "]);
        obj.setCollisionBool(objJson["collisionBool: "]);
	}
}
void save::loadGameObjects(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    json j;
    file >> j;
    file.close();

    gameObject::gameObjects.clear();

    // Check if "gameObjects" exists and is an array
    if (!j.contains("gameObjects") || !j["gameObjects"].is_array()) {
        std::cerr << "Invalid JSON: 'gameObjects' key missing or not an array" << std::endl;
        return;
    }

	std::cout << "Loaded JSON: " << j.dump(4) << std::endl;  // Pretty-print with indent 4

    for (auto& objJson : j["gameObjects"]) {
        gameObject obj;

        // For each expected key, check if it exists and is not null.
        if (objJson.contains("pos") && objJson["pos"].is_array() && objJson["pos"].size() >= 2) {
            // Optionally, you can also check that the elements are numbers.
            if (!objJson["pos"][0].is_number() || !objJson["pos"][1].is_number()) {
                std::cerr << "Invalid 'pos' values; expected numbers." << std::endl;
                continue;  // or set default values
            }
            obj.setPos(objJson["pos"][0], objJson["pos"][1]);
        } else {
            std::cerr << "'pos' key missing or invalid in one of the game objects" << std::endl;
            // Set default values or skip this object
            obj.setPos(0, 0);
        }

        if (objJson.contains("vel") && objJson["vel"].is_array() && objJson["vel"].size() >= 2) {
            if (!objJson["vel"][0].is_number() || !objJson["vel"][1].is_number()) {
                std::cerr << "Invalid 'vel' values; expected numbers." << std::endl;
                continue;
            }
            obj.setVel(objJson["vel"][0], objJson["vel"][1]);
        } else {
            std::cerr << "'vel' key missing or invalid" << std::endl;
            obj.setVel(0, 0);
        }

        if (objJson.contains("dim") && objJson["dim"].is_array() && objJson["dim"].size() >= 2) {
            if (!objJson["dim"][0].is_number() || !objJson["dim"][1].is_number()) {
                std::cerr << "Invalid 'dim' values; expected numbers." << std::endl;
                continue;
            }
            obj.setDim(objJson["dim"][0], objJson["dim"][1]);
        } else {
            std::cerr << "'dim' key missing or invalid" << std::endl;
            obj.setDim(0, 0);
        }

        // For non-array values, check type or set default.
        if (objJson.contains("rotation") && !objJson["rotation"].is_null() && objJson["rotation"].is_number()) {
            obj.setRotation(objJson["rotation"]);
        } else {
            std::cerr << "'rotation' key missing or not a number" << std::endl;
            obj.setRotation(0);
        }

        if (objJson.contains("gravity") && !objJson["gravity"].is_null() && objJson["gravity"].is_boolean()) {
            obj.setGravityBool(objJson["gravity"]);
        } else {
            std::cerr << "'gravity' key missing or not a boolean" << std::endl;
            obj.setGravityBool(false);
        }

        if (objJson.contains("collision") && !objJson["collision"].is_null() && objJson["collision"].is_boolean()) {
            obj.setCollisionBool(objJson["collision"]);
        } else {
            std::cerr << "'collision' key missing or not a boolean" << std::endl;
            obj.setCollisionBool(false);
        }

        gameObject::gameObjects.push_back(obj);
    }
}
*/
