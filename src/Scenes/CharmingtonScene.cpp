#include "CharmingtonScene.h"

namespace EWE {
	CharmingtonScene::CharmingtonScene(EightWindsEngine& ewEngine)
		: ewEngine{ ewEngine },
		menuManager{ ewEngine.menuManager },
		soundEngine{ SoundEngine::getSoundEngineInstance() },
		levelManager{ewEngine.eweDevice},
		charmer{ewEngine.mainWindow.getGLFWwindow(), ewEngine.camera}
	{}
	CharmingtonScene::~CharmingtonScene() {
		printf("deconstructing main menu \n");
	}


	void CharmingtonScene::load() {



		ewEngine.advancedRS.updatePipelines(ewEngine.objectManager, ewEngine.eweRenderer.getPipelineInfo());
		printf("after updating pipelines load menu objects, returning \n");
	}
	void CharmingtonScene::entry() {
		soundEngine->stopMusic();
		//soundEngine->playMusic(Music_Menu);

		//menuManager.changeMenuState(menu_main, 0);

		//old method
		/*
		ewEngine.camera.setViewTarget({ 40.f, 0.f, 40.0f }, { 0.f, 0.f, 0.f }, glm::vec3(0.f, 1.f, 0.f));
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			ewEngine.camera.bindUBO(i);
		}
		*/
		//new camera method
		levelManager.initLevel(ewEngine.eweDevice);

		logicActive = true;
		logicThread = std::make_unique<std::thread>(&CharmingtonScene::logicThreadFunction, this);

		//handle threads in this scene, or a game specific class
	}
	void CharmingtonScene::exit() {
		logicActive = false;
		logicThread->join();
		ewEngine.objectManager.eweObjects.clear();
	}

	void CharmingtonScene::logicThreadFunction() {
		auto logicThreadCurrentTime = std::chrono::high_resolution_clock::now();
		const double logicUpdateRate = 0.004;
		auto newTime = std::chrono::high_resolution_clock::now();
		double logicThreadTimeTracker = 0.0;

		while (logicActive) {
			newTime = std::chrono::high_resolution_clock::now();
			logicThreadTimeTracker += std::chrono::duration<double, std::chrono::seconds::period>(newTime - logicThreadCurrentTime).count();
			logicThreadCurrentTime = newTime;

			if (logicThreadTimeTracker >= logicUpdateRate) {
				logicThreadTimeTracker -= logicUpdateRate;
				charmer.logicUpdate();
			}
		}
	}

	bool CharmingtonScene::render(double dt) {
		//printf("render main menu scene \n");


		auto cmdBufFrameIndex = ewEngine.beginRender();
		if (cmdBufFrameIndex.first != VK_NULL_HANDLE) {
			//printf("drawing \n");
			ewEngine.drawObjects(cmdBufFrameIndex, dt);
			FrameInfo frameInfo;
			frameInfo.cmdIndexPair = cmdBufFrameIndex;
			frameInfo.time = static_cast<float>(dt);
			levelManager.renderLevel(frameInfo);
			//printf("after displaying render info \n");
			ewEngine.endRender(cmdBufFrameIndex);
			//std::cout << "after ending render \n";
			return false;
		}
		return true;
	}
}