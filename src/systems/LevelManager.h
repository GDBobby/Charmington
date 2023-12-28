#pragma once

#include "EWEngine/GUI/UIHandler.h"

#include "EWEngine/EWE_GameObject.h"
#include "EWEngine/SoundEngine.h"
//#include "VoidsGaze/GUI/EndlessOverlay.h"

#include "EWEngine/systems/SkinRendering/SkinRenderSystem.h"
#include "EWEngine/systems/RigidRendering/RigidRenderSystem.h"
#include "EWEngine/graphics/EWE_FrameInfo.h"


namespace EWE {
	class LevelManager {
	public:
		LevelManager(EWEDevice& device, uint8_t maxFIF, SkinRenderSystem& skinnedRS);

		~LevelManager() {

		}

		void loadStage();

		void updateLogic();
		void renderUpdate();
		void stepDeathRow();

		bool updatePipes();


		void swapNextStage();
		//void writeToBuffer(std::vector<std::unique_ptr<EWEBuffer>>* bufferVector, uint8_t frameIndex);
		void destroyStage(bool returningToMain);

		void drawLevel(FrameInfo& frameInfo);


		LevelValues levelValues;
		EWEGameObject background;
		std::vector<EWEGameObject> accessoryBackground;
		bool drawingBackground = false;

		//std::shared_ptr<LevelUI> corruptionUI;

		std::shared_ptr<SoundEngine> soundEngine;

		void reset() {
			printf("~~~~~~~~~~~~~~~~~ RESETTING LEVEL MANAGER ~~~~~~~~~~~~~~~~~~~~~~~ \n");
		}

	protected:

		EWEDevice& device;
	};

}