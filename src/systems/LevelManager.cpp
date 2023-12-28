#include "LevelManager.h"
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

namespace EWE {
	LevelManager::LevelManager(EWEDevice& device, Charmer& charmer) : device{ device }, soundEngine{ SoundEngine::getSoundEngineInstance() }, charmer{ charmer } {

		populateLevels();
	}
	LevelManager::~LevelManager() {
	}

	void LevelManager::initLevel(EWEDevice& device) {
		currentLevel->enterLevel(device);
		charmer.currentLevel = currentLevel;
	}
	void LevelManager::logicUpdate() {
		charmer.logicUpdate();
	}

	void LevelManager::renderLevel(FrameInfo& frameInfo) {
		currentLevel->render(frameInfo);
	}
	void LevelManager::swapNextStage(LevelID levelID) {
		currentLevel->exitLevel();
		currentLevel = levels.at(levelID).get();
		currentLevel->enterLevel(device);
		charmer.currentLevel = currentLevel;
	}

	void LevelManager::populateLevels() {

		levels.emplace(0, std::make_unique<FirstLevel>(device));
		currentLevel = levels.at(0).get();
	}
}