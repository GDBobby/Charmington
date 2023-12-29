#include "LevelManager.h"
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

namespace EWE {
	LevelManager::LevelManager(EightWindsEngine& ewEngine, EWEDevice& device, Charmer& charmer) : device{ device }, soundEngine{ SoundEngine::getSoundEngineInstance() }, charmer{ charmer }, ewEngine{ewEngine} {

		populateLevels();
	}
	LevelManager::~LevelManager() {
	}

	void LevelManager::initLevel(EWEDevice& device) {
		currentLevel->enterLevel(device);
		ewEngine.advancedRS.updatePipelines(ewEngine.eweRenderer.getPipelineInfo());
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
		levels.emplace(1, std::make_unique<StartLevel>(device));
		currentLevel = levels.at(1).get();
	}
}