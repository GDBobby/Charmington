#pragma once
#include "../systems/Level.h"

namespace EWE {
	class FirstLevel : public Level {
	public:
		FirstLevel(EWEDevice& device) : Level{TileSet::TS_First} {
			floorTransform.scale.x = 50.f;
			floorTransform.scale.z = 50.f;
		}
		void enterLevel(EWEDevice& device) override;


	};
}
