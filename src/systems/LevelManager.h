#pragma once

#include "EWEngine/GUI/UIHandler.h"

#include "EWEngine/EWE_GameObject.h"
#include "EWEngine/SoundEngine.h"
//#include "VoidsGaze/GUI/EndlessOverlay.h"

#include "EWEngine/systems/SkinRendering/SkinRenderSystem.h"
#include "EWEngine/systems/RigidRendering/RigidRenderSystem.h"
#include <EWEngine/EightWindsEngine.h>
#include "EWEngine/graphics/EWE_FrameInfo.h"
#include "../Characters/Charmer.h"

#include "Level.h"
#include "../Levels/FirstLevel.h"
#include "../Levels/StartArea.h"
#include "../Levels/ConnectorLevel.h"
#include "../Levels/ForestLevel.h"


namespace EWE {
	class LevelManager {
	public:
		LevelManager(EightWindsEngine& ewEngine, EWEDevice& device, Charmer& charmer);

		~LevelManager();

		void loadStage();

		void updateLogic();
		void renderUpdate();
		void stepDeathRow();

		bool updatePipes();

		void initLevel(EWEDevice& device);

		void swapNextStage(LevelID levelID);
		//void writeToBuffer(std::vector<std::unique_ptr<EWEBuffer>>* bufferVector, uint8_t frameIndex);
		void destroyStage(bool returningToMain);

		void renderLevel(FrameInfo& frameInfo);

		//LevelValues levelValues;
		EWEGameObject background;
		std::vector<EWEGameObject> accessoryBackground;
		bool drawingBackground = false;

		//std::shared_ptr<LevelUI> corruptionUI;

		std::shared_ptr<SoundEngine> soundEngine;

		void reset() {
			printf("~~~~~~~~~~~~~~~~~ RESETTING LEVEL MANAGER ~~~~~~~~~~~~~~~~~~~~~~~ \n");
		}
		void logicUpdate();

		bool waitingForRender = false;
		bool waitingForLogic = false;

	protected:
		Charmer& charmer;

		Level* currentLevel = 0;
		LevelID currentLevelID = Level::Level_Start;

		std::unordered_map<LevelID, std::unique_ptr<Level>> levels{};

		EWEDevice& device;
		EightWindsEngine& ewEngine;

		void populateLevels();

	};

}