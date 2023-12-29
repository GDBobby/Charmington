#include "StartArea.h"

namespace EWE {
	void StartLevel::enterLevel(EWEDevice& device) {
		std::string textureLocation{ "startArea.png" };
		std::string tileMapLocation{ "models/startArea.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
	}
}