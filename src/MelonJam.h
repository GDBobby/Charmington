#pragma once

#include <EWEngine/EightWindsEngine.h>

#include "Scenes/SceneEnum.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/CharmingtonScene.h"

#include "pipelines/BackgroundPipe.h"
#include "pipelines/GrassPipe.h"
#include "pipelines/BillboardPipe.h"
#include "pipelines/PipelineEnum.h"
#include "MusicEnum.h"

//#include "Scenes/FreeCameraScene.h"
#include <functional>

namespace EWE {


	class MelonJam {
	public:
		MelonJam(EightWindsEngine& ewEngine);
		~MelonJam();
		GLFWwindow* windowPtr;
		EightWindsEngine& ewEngine;
		MenuManager& menuManager;

		Scene_Enum lastScene = scene_mainmenu;
		Scene_Enum currentScene = scene_mainmenu;
		std::unordered_map<Scene_Enum, std::unique_ptr<Scene>> scenes;
		Scene* currentScenePtr{ nullptr };
		bool swappingScenes = false;
		std::shared_ptr<SoundEngine> soundEngine;

		bool gameRunning = true;
		double renderRefreshRate = 0.0;

		void mainThread();
		void addModulesToMenuManager(float screenWidth, float screenHeight);
		
		void loadGlobalObjects();
		bool processClick();

		void addPipelinesToSystem();
		void addSound();



	};
}