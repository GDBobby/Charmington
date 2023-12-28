#pragma once
#include "../systems/Level.h"

namespace EWE {
	class FirstLevel : public Level {
	public:
		FirstLevel(EWEDevice& device) : Level{ } {
			floorTransform.scale.x = 100.f;
			floorTransform.scale.z = 100.f;
		}
		void enterLevel(EWEDevice& device) override {
			floorTextureID = EWETexture::addSceneTexture(device, "map.png");
		}


	};
}
