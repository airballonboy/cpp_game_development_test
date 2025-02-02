#pragma once
#include "gameObject.hpp"
#include <string>

namespace save{
	void saveGameObjects(const std::string& filename);
	void loadGameObjects(const std::string& filename);
}

