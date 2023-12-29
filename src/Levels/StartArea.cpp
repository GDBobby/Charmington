#include "StartArea.h"

namespace EWE {
	StartLevel::StartLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_First);
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Start, entryTransform);

		entryTransform.translation.x = -10.f;
		entryTransform.translation.y = 0.f;
		entryTransform.translation.z = -0.5f;

		entryTransform.rotation.y = 0.f;

		entryPoints.emplace(Level::Level_First, entryTransform);

		entryTransform.translation.x = 0.f;
		entryTransform.translation.y = 0.f;
		entryTransform.translation.z = -10.0f;

		entryPoints.emplace(Level::Level_Connector, entryTransform);
	}
	void StartLevel::exitLevel() {
		//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		rock.reset();
		Level::exitLevel();


	}

	void StartLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "startArea.png" };
		std::string tileMapLocation{ "models/startArea.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);


		rock.reset(new EweObject("rock", device, false));
		rock->transform.translation.z = -11.f;
	}
}