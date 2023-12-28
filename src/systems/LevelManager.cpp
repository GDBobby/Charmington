#include "LevelManager.h"
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

namespace EWE {
	LevelManager::LevelManager(EWEDevice& device) : device{ device }, soundEngine{SoundEngine::getSoundEngineInstance()} {
		Level::floor = Basic_Model::generateSimple3DQuad(device);

		populateLevels();
	}
	LevelManager::~LevelManager() {
		Level::floor.reset();
	}

	void LevelManager::initLevel(EWEDevice& device) {
		levels.at(currentLevel)->enterLevel(device);
	}

	void LevelManager::renderLevel(FrameInfo& frameInfo) {
		levels.at(currentLevel)->render(frameInfo);
	}

	void LevelManager::populateLevels() {

		levels.emplace(0, std::make_unique<FirstLevel>(device));
	}
}