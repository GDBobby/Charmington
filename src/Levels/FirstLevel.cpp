#include "FirstLevel.h"

namespace EWE {
	void FirstLevel::enterLevel(EWEDevice& device) {
		std::string textureLocation{ "firstArena.png" };
		std::string tileMapLocation{ "models/firstArena.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);

		rock.reset(new EweObject("rock", device, false));
	}
}