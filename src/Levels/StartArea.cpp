#include "StartArea.h"

namespace EWE {
	void StartLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "startArea.png" };
		std::string tileMapLocation{ "models/startArea.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);


		rock.reset(new EweObject("rock", device, false));
		rock->transform.translation.z = -11.f;
	}
}