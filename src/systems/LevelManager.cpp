#include "LevelManager.h"
#include <EWEngine/Graphics/Model/Basic_Model.h>

#include "../GUI/MenuEnums.h"

namespace EWE {
	LevelManager::LevelManager(EightWindsEngine& ewEngine, EWEDevice& device, Charmer& charmer) : device{ device }, soundEngine{ SoundEngine::getSoundEngineInstance() }, charmer{ charmer }, ewEngine{ewEngine} {

		populateLevels();
	}
	LevelManager::~LevelManager() {
		currentLevel->exitLevel();
	}

	void LevelManager::initLevel(EWEDevice& device) {

		currentLevel->enterLevel(device);
		ewEngine.advancedRS.updatePipelines();
		charmer.currentLevel = currentLevel;
		if (currentLevelID == Level::Level_First) {
			if (((FirstLevel*)currentLevel)->carrot.get() != nullptr) {
				((FirstLevel*)currentLevel)->carrot->charmerTranslation = charmer.getTranslationPtr();
			}
		}
		else if (currentLevelID == Level::Level_WoodChop) {
			if (((ForestLevel*)currentLevel)->zero.get() != nullptr) {
				((ForestLevel*)currentLevel)->zero->charmerTranslation = charmer.getTranslationPtr();
			}
		}
		charmerOverlay->updateFromSettings();
		paused = false;
	}
	void LevelManager::logicUpdate() {

		if(charmer.wantsMenu()) {
			ewEngine.menuManager.giveMenuFocus();
			paused = true;
		}
		if (paused) {
			printf("paused \n");
			return;
		}

		charmer.logicUpdate();
		int32_t changeBuffer = charmer.getLevelChange();

		//printf("change buffer in level manager : %d \n", changeBuffer);
		if (changeBuffer >= 0) {

			printf("swapping stage at request of charmer : %d \n", changeBuffer);
			swapNextStage(currentLevel->exits[changeBuffer]);
		}
		switch (currentLevelID) {
			case Level::Level_Start: {
				//StartLevel* startArea = ((StartLevel*)currentLevel);
				//if()
				break;
			}
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
						charmerOverlay->updateFromSettings();
					}
				}

				break;
			}

			case Level::Level_WoodChop: {
				ForestLevel* forestLevel = ((ForestLevel*)currentLevel);
				if (forestLevel->zero.get() != nullptr) {
					forestLevel->zero->logicUpdate();

					if (forestLevel->zero->tamed) {
						printf("ZERO WAS TAMED \n");
						SaveJSON::saveData.petFlags |= SaveJSON::PetFlags::PF_Zero;
						SaveJSON::saveToJsonFile();
						waitingForRender = true;
						waitingForLogic = true;
						while (waitingForRender) {
							printf("waiting for render thread \n");
							std::this_thread::sleep_for(std::chrono::milliseconds(10));
						}
						charmer.tamedZero(device, forestLevel->zero->skeleton);
						forestLevel->zero.reset(nullptr);
						waitingForLogic = false;
						charmerOverlay->updateFromSettings();
					}
				}
				break;
			}
			case Level::Level_SpookyForest: {
				SpookyForest* spookyForest = ((SpookyForest*)currentLevel);
				if (spookyForest->sheet.get() != nullptr) {
					spookyForest->sheet->logicUpdate();
					if (spookyForest->sheet->transform.translation.z < -8.5f) {
						ewEngine.menuManager.changeMenuState(menu_end);
						ewEngine.menuManager.giveMenuFocus();
						paused = true;
					}
				}
				break;
			}
			case Level::Level_Connector: {
				ConnectorLevel* connector = ((ConnectorLevel*)currentLevel);
				if (connector->sheet.get() != nullptr) {
					connector->sheet->logicUpdate();
					if (connector->scaredSheet && connector->sheet->escaped) {
						waitingForRender = true;
						waitingForLogic = true;
						while (waitingForRender) {
							printf("waiting for render thread \n");
							std::this_thread::sleep_for(std::chrono::milliseconds(10));
						}
						
						connector->sheet.reset(nullptr);
						waitingForLogic = false;
						SaveJSON::saveData.obstacleFlags |= SaveJSON::ObstacleFlags::OF_Sheet;
						SaveJSON::saveToJsonFile();
						charmerOverlay->updateFromSettings();
					}
				}
				break;
			}
			default: {
				break;
			}
		}
	}

	void LevelManager::renderLevel(FrameInfo const& frameInfo, float dt) {
		currentLevel->render(frameInfo, dt);

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
		currentLevel->enterLevel(device);
		charmer.setTransform(currentLevel->getEntryTransform(currentLevelID));
		currentLevelID = levelID;
		charmer.currentLevel = currentLevel;
		ewEngine.advancedRS.updatePipelines();
		if (currentLevelID == Level::Level_First) {
			if (((FirstLevel*)currentLevel)->carrot.get() != nullptr) {
				((FirstLevel*)currentLevel)->carrot->charmerTranslation = charmer.getTranslationPtr();
			}
		}
		else if (currentLevelID == Level::Level_WoodChop) {
			if (((ForestLevel*)currentLevel)->zero.get() != nullptr) {
				((ForestLevel*)currentLevel)->zero->charmerTranslation = charmer.getTranslationPtr();
			}
		}

		printf("finished swapping stage \n");
		waitingForLogic = false;
		SaveJSON::saveToJsonFile();

		charmerOverlay->updateFromSettings();
	}

	void LevelManager::populateLevels() {
		printf("before loading levels \n");

		levels.emplace(Level::Level_Start, std::make_unique<StartLevel>(device));
		levels.emplace(Level::Level_First, std::make_unique<FirstLevel>(device));
		levels.emplace(Level::Level_Connector, std::make_unique<ConnectorLevel>(device));
		levels.emplace(Level::Level_WoodChop, std::make_unique<ForestLevel>(device));
		levels.emplace(Level::Level_SpookyForest, std::make_unique<SpookyForest>(device));
		printf("after loading levels : %d\n", SaveJSON::saveData.currentMap);
		currentLevel = levels.at(SaveJSON::saveData.currentMap).get();
		currentLevelID = SaveJSON::saveData.currentMap;
		LevelID entranceID = SaveJSON::saveData.currentEntrance;
		charmer.setTransform(currentLevel->getEntryTransform(entranceID));
	}
	void LevelManager::exitScene() {
		currentLevel->exitLevel();
		
	}
}