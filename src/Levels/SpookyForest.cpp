#include "SpookyForest.h"
#include <EWEngine/SoundEngine.h>
#include "../MusicEnum.h"

namespace EWE {
	SpookyForest::SpookyForest(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.translation.x = 0.f;
		entryTransform.translation.z = 0.f;
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Connector, entryTransform);
	}

	void SpookyForest::exitLevel() {
		//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		trees.clear();
		Level::exitLevel();
		sheet.reset();

		SoundEngine::getSoundEngineInstance()->stopMusic();
		SoundEngine::getSoundEngineInstance()->playMusic(Music_menu, true);
	}

	void SpookyForest::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "SpookyForest.png" };
		std::string tileMapLocation{ "models/SpookyForest.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
		sheet = std::make_unique<Sheet>(device, globalPool);

		SoundEngine::getSoundEngineInstance()->stopMusic();
		SoundEngine::getSoundEngineInstance()->playMusic(Music_spookyForest, true);
	}

	void SpookyForest::render(FrameInfo& frameInfo) {
		Level::render(frameInfo);
		sheet->renderUpdate();
	}
}