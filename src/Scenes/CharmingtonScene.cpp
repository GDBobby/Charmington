#include "CharmingtonScene.h"

#include "../MusicEnum.h"

namespace EWE {
	CharmingtonScene::CharmingtonScene(EightWindsEngine& ewEngine)
		: ewEngine{ ewEngine },
		menuManager{ ewEngine.menuManager },
		soundEngine{ SoundEngine::getSoundEngineInstance() },
		charmer{ ewEngine.eweDevice, ewEngine.mainWindow.getGLFWwindow(), ewEngine.camera},
		levelManager{ewEngine, ewEngine.eweDevice, charmer},
		charmerOverlay{std::make_shared<CharmerOverlay>(ewEngine.eweDevice, ewEngine.menuManager.screenWidth, ewEngine.menuManager.screenHeight, charmer.logCount)}
	{
		CharmerInput::giveCallbackReturns(ewEngine.menuManager.staticMouseCallback, ewEngine.menuManager.staticKeyCallback);

		ewEngine.uiHandler.overlay = charmerOverlay;
		levelManager.charmerOverlay = charmerOverlay;
		charmerOverlay->setActive(false);
	}
	CharmingtonScene::~CharmingtonScene() {
		printf("deconstructing main menu \n");
	}


	void CharmingtonScene::load() {



		ewEngine.advancedRS.updatePipelines();
		printf("after updating pipelines load menu objects, returning \n");
	}
	void CharmingtonScene::entry() {
		//soundEngine->stopMusic();

		soundEngine->stopMusic();
		soundEngine->playMusic(Music_forest, true);

		//menuManager.changeMenuState(menu_main, 0);

		//old method
		/*
		ewEngine.camera.setViewTarget({ 40.f, 0.f, 40.0f }, { 0.f, 0.f, 0.f }, glm::vec3(0.f, 1.f, 0.f));
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			ewEngine.camera.bindUBO(i);
		}
		*/
		LightBufferObject lbo;
		lbo.ambientColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);
		lbo.sunlightDirection = { 1.f, 3.f, 1.f, 0.f };
		lbo.sunlightColor = glm::vec4{ 0.25f, 0.25f, 0.25f, 0.5f };
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			ewEngine.bufferMap[Buff_gpu][i]->writeToBuffer(&lbo);
			ewEngine.bufferMap[Buff_gpu][i]->flush();
		}

		//new camera method
		levelManager.initLevel(ewEngine.eweDevice);
		charmer.giveInputFocus();
		menuManager.closeMenu();
		logicActive = true;
		logicThread = std::make_unique<std::thread>(&CharmingtonScene::logicThreadFunction, this);
		charmerOverlay->setActive(true);
		//handle threads in this scene, or a game specific class
	}
	void CharmingtonScene::exit() {
		charmerOverlay->setActive(false);
		logicActive = false;
		if (logicThread.get() != nullptr) {
			logicThread->join();
		}
		levelManager.exitScene();
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
				levelManager.logicUpdate();
			}
		}
	}

	bool CharmingtonScene::render(double dt) {
		//printf("render main menu scene \n");
		if (charmer.wantsMenu()) {
			menuManager.giveMenuFocus();
		}

		if (levelManager.waitingForRender) {
			printf("waiting for idle queue \n");
			vkQueueWaitIdle(ewEngine.eweDevice.graphicsQueue());
			levelManager.waitingForRender = false;
			while (levelManager.waitingForLogic) {
				printf("waiting for logic thread \n");
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		FrameInfo frameInfo{ ewEngine.beginRender() };
		if (frameInfo.cmdBuf != VK_NULL_HANDLE) {
			//printf("charmington render? \n");
			//printf("drawing \n");
			//ewEngine.drawObjects(cmdBufFrameIndex, dt);

			PipelineSystem::setFrameInfo(frameInfo);
			ewEngine.draw3DObjects(frameInfo, dt);
			charmer.renderUpdate();
			levelManager.renderLevel(frameInfo, dt);
#if RENDER_TRANSLATION
			ewEngine.uiHandler.beginTextRender();
			if (ewEngine.displayingRenderInfo) {
				charmer.drawTranslation();
				ewEngine.uiHandler.Benchmarking(dt, ewEngine.peakRenderTime, ewEngine.averageRenderTime, ewEngine.highestRenderTime, ewEngine.averageLogicTime, BENCHMARKING_GPU, ewEngine.elapsedGPUMS, ewEngine.averageElapsedGPUMS);
			}
			ewEngine.draw2DObjects(frameInfo);
			ewEngine.uiHandler.drawOverlayText(frameInfo.cmdBuf, ewEngine.displayingRenderInfo);
			menuManager.drawText();
			ewEngine.uiHandler.endTextRender(frameInfo.cmdBuf);
#else

			ewEngine.drawText(cmdBufFrameIndex, dt);
#endif
			//printf("after displaying render info \n");
			ewEngine.endRender(frameInfo);
			//std::cout << "after ending render \n";
			return false;
		}
		return true;
	}
}