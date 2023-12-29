#include "LevelManager.h"
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

namespace EWE {
	LevelManager::LevelManager(EightWindsEngine& ewEngine, EWEDevice& device, Charmer& charmer) : device{ device }, soundEngine{ SoundEngine::getSoundEngineInstance() }, charmer{ charmer }, ewEngine{ewEngine} {

		populateLevels();
	}
	LevelManager::~LevelManager() {
	}

	void LevelManager::initLevel(EWEDevice& device) {
		currentLevel->enterLevel(device, ewEngine.advancedRS.globalPool);
		ewEngine.advancedRS.updatePipelines(ewEngine.eweRenderer.getPipelineInfo());
		charmer.currentLevel = currentLevel;
		if (currentLevelID == Level::Level_First) {
			if (((FirstLevel*)currentLevel)->carrot.get() != nullptr) {
				((FirstLevel*)currentLevel)->carrot->charmerTranslation = charmer.getTranslationPtr();
			}
		}
	}
	void LevelManager::logicUpdate() {
		charmer.logicUpdate();
		int32_t changeBuffer = charmer.getLevelChange();
		//printf("change buffer in level manager : %d \n", changeBuffer);
		if (changeBuffer >= 0) {

			printf("swapping stage at request of charmer : %d \n", changeBuffer);
			swapNextStage(currentLevel->exits[changeBuffer]);
		}
		switch (currentLevelID) {
			case Level::Level_First:{
				FirstLevel* firstLevel = ((FirstLevel*)currentLevel);
				if (firstLevel->carrot.get() != nullptr) {
					firstLevel->carrot->logicUpdate();
					int8_t checkHit = firstLevel->carrot->checkHit();
					if (checkHit == 0) {
						printf("sent out of stage by carrot \n");
						swapNextStage(Level::Level_Start);
						printf("after swapping stage by carrot \n");
						return;
					}
					else if (checkHit > 0) {
						printf("dropping apple : %d \n", checkHit);

						firstLevel->dropApple(device, checkHit - 1);
					}
					int8_t ateAppleID = firstLevel->carrot->ateApple();
					if (ateAppleID > -1) {
						printf("eating apple \n");
						firstLevel->eatApple(ateAppleID);
					}


					if (firstLevel->carrot->tamed) {
						printf("CARROT WAS TAMED \n");
						SaveJSON::saveData.petFlags |= SaveJSON::PetFlags::PF_Carrot;
						SaveJSON::saveToJsonFile();
						waitingForRender = true;
						waitingForLogic = true;
						while (waitingForRender) {
							printf("waiting for render thread \n");
							std::this_thread::sleep_for(std::chrono::milliseconds(10));
						}
						charmer.tamedCarrot(device, firstLevel->carrot->skeleton);
						firstLevel->unloadCarrot();
						waitingForLogic = false;
					}
				}

				break;
			}
			
		}
	}

	void LevelManager::renderLevel(FrameInfo& frameInfo) {
		currentLevel->render(frameInfo);

	}
	void LevelManager::swapNextStage(LevelID levelID) {

		SaveJSON::saveData.currentMap = levelID;
		SaveJSON::saveData.currentEntrance = currentLevelID;

		waitingForRender = true;
		waitingForLogic = true;
		while (waitingForRender) {
			printf("waiting for render thread \n");
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		printf("swapstage : %d \n", levelID);
		currentLevel->exitLevel();
		currentLevel = levels.at(levelID).get();
		currentLevel->enterLevel(device, ewEngine.advancedRS.globalPool);
		charmer.setTransform(currentLevel->getEntryTransform(currentLevelID));
		currentLevelID = levelID;
		charmer.currentLevel = currentLevel;
		ewEngine.advancedRS.updatePipelines(ewEngine.eweRenderer.getPipelineInfo());
		if (currentLevelID == Level::Level_First) {
			if (((FirstLevel*)currentLevel)->carrot.get() != nullptr) {
				((FirstLevel*)currentLevel)->carrot->charmerTranslation = charmer.getTranslationPtr();
			}
		}

		printf("finished swapping stage \n");
		waitingForLogic = false;
		SaveJSON::saveToJsonFile();
	}

	void LevelManager::populateLevels() {
		printf("before loading levels \n");

		levels.emplace(Level::Level_Start, std::make_unique<StartLevel>(device));
		levels.emplace(Level::Level_First, std::make_unique<FirstLevel>(device));
		printf("after loading levels : %d\n", SaveJSON::saveData.currentMap);
		currentLevel = levels.at(SaveJSON::saveData.currentMap).get();
		currentLevelID = SaveJSON::saveData.currentMap;
		LevelID entranceID = SaveJSON::saveData.currentEntrance;
		charmer.setTransform(currentLevel->getEntryTransform(entranceID));
	}
}