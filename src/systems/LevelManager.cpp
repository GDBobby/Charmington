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
		int32_t changeBuffer = charmer.getLevelChange();
		//printf("change buffer in level manager : %d \n", changeBuffer);
		if (changeBuffer >= 0) {

			printf("swapping stage at request of charmer : %d \n", changeBuffer);

			waitingForRender = true;
			waitingForLogic = true;
			while (waitingForRender) {
				printf("waiting for render thread \n");
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			swapNextStage(currentLevel->exits[changeBuffer]);
			printf("finished swapping stage \n");
			waitingForLogic = false;
		}
	}

	void LevelManager::renderLevel(FrameInfo& frameInfo) {
		currentLevel->render(frameInfo);
	}
	void LevelManager::swapNextStage(LevelID levelID) {
		printf("swapstage : %d \n", levelID);
		currentLevel->exitLevel();
		currentLevel = levels.at(levelID).get();
		currentLevel->enterLevel(device);
		charmer.setTransform(currentLevel->getEntryTransform(currentLevelID));
		currentLevelID = levelID;
		charmer.currentLevel = currentLevel;
		ewEngine.advancedRS.updatePipelines(ewEngine.eweRenderer.getPipelineInfo());
	}

	void LevelManager::populateLevels() {
		printf("before loading levels \n");
		levels.emplace(Level::Level_Start, std::make_unique<StartLevel>(device));
		levels.emplace(Level::Level_First, std::make_unique<FirstLevel>(device));
		printf("after loading levels : %d\n", SaveJSON::saveData.currentMap);
		currentLevel = levels.at(SaveJSON::saveData.currentMap).get();
		currentLevelID = SaveJSON::saveData.currentMap;
	}
}