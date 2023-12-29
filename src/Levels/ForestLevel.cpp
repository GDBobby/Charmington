#include "ForestLevel.h"

namespace EWE {
	ForestLevel::ForestLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.translation.x = -11.5f;
		entryTransform.translation.z = -2.f;
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Connector, entryTransform);
	}

	void ForestLevel::exitLevel() {
		//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		trees.clear();
		Level::exitLevel();
	}

	void ForestLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "WoodChop.png" };
		std::string tileMapLocation{ "models/WoodChop.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
	}
}