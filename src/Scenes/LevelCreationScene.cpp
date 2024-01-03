#include "LevelCreationScene.h"

#include "../pipelines/PipelineEnum.h"

namespace EWE {
	LevelCreationScene* LevelCreationScene::lcPtr{nullptr};

	LevelCreationScene::LevelCreationScene(EightWindsEngine& ewEngine)
		: ewEngine{ ewEngine },
		menuManager{ ewEngine.menuManager },
		soundEngine{ SoundEngine::getSoundEngineInstance() },
		levelCreationIMGUI{MenuModule::clickReturns, menuManager.screenWidth, menuManager.screenHeight}
	{
		lcPtr = this;
	}
	LevelCreationScene::~LevelCreationScene() {
		printf("deconstructing main menu \n");
		lcPtr = nullptr;
	}

	void LevelCreationScene::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse) {
			lcPtr->levelCreationIMGUI.mouseCallback(button, action);
			//printf("imgui wants mouse capture \n");
			return;
		}
	}
	void LevelCreationScene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureKeyboard) {
			//printf("imgui wants mouse capture \n");
			return;
		}
	}


	void LevelCreationScene::load() {
		menuManager.giveMenuFocus();



		ewEngine.advancedRS.updatePipelines(ewEngine.eweRenderer.getPipelineInfo());
		printf("after updating pipelines load menu objects, returning \n");
	}
	void LevelCreationScene::entry() {
		soundEngine->stopMusic();

		menuManager.changeMenuState(menu_main, 0);
		menuManager.closeMenu();

		//old method
		/*
		ewEngine.camera.setViewTarget({ 40.f, 0.f, 40.0f }, { 0.f, 0.f, 0.f }, glm::vec3(0.f, 1.f, 0.f));
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			ewEngine.camera.bindUBO(i);
		}
		*/
		//new camera method
		for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			ewEngine.camera.updateViewData({ 40.f, 0.f, 40.0f }, { 0.f, 0.f, 0.f }, glm::vec3(0.f, 1.f, 0.f));
		}
		imguiHandler = std::make_unique<ImGUIHandler>(ewEngine.mainWindow.getGLFWwindow(), ewEngine.eweDevice, MAX_FRAMES_IN_FLIGHT, ewEngine.eweRenderer.getPipelineInfo());
		//handle threads in this scene, or a game specific class
		ewEngine.advancedRS.drawSkybox = false;
		levelCreationIMGUI.gridScale = &pushScale;
		levelCreationIMGUI.gridTrans = &pushTrans;
		pushScale.x = menuManager.screenWidth / menuManager.screenHeight;
		pushScale.y = 1.f;
		pushTrans.x = 0.f;
		pushTrans.y = 0.f;
		levelCreationIMGUI.tileSetID = EWETexture::addGlobalTexture(ewEngine.eweDevice, "tileSet.png");

		glfwSetMouseButtonCallback(ewEngine.mainWindow.getGLFWwindow(), LevelCreationScene::mouseCallback);
		glfwSetKeyCallback(ewEngine.mainWindow.getGLFWwindow(), LevelCreationScene::keyCallback);
		printf("tileSetID : %d \n", levelCreationIMGUI.tileSetID);
	}
	void LevelCreationScene::exit() {
		ewEngine.objectManager.eweObjects.clear();
		imguiHandler.reset();
		ewEngine.advancedRS.drawSkybox = true;
		glfwSetMouseButtonCallback(ewEngine.mainWindow.getGLFWwindow(), mouseReturnFunction);
		glfwSetKeyCallback(ewEngine.mainWindow.getGLFWwindow(), keyReturnFunction);
	}
	bool LevelCreationScene::render(double dt) {
		//printf("render main menu scene \n");


		auto cmdBufFrameIndex = ewEngine.beginRender();

		if (cmdBufFrameIndex.first != VK_NULL_HANDLE) {

			imguiHandler->beginRender();
			//printf("drawing \n");
			renderBackgroundGrid(cmdBufFrameIndex);
			//ewEngine.drawObjects(cmdBufFrameIndex, dt);
			ewEngine.drawText(cmdBufFrameIndex, dt);
			FrameInfo frameInfo;
			frameInfo.cmdIndexPair = cmdBufFrameIndex;
			frameInfo.time = static_cast<float>(dt);
			//printf("after displaying render info \n");
			ImGui::ShowDemoWindow(&show_demo_window);

			levelCreationIMGUI.render(cmdBufFrameIndex.second);
			imguiHandler->endRender(cmdBufFrameIndex.first);
			ewEngine.endRender(cmdBufFrameIndex);
			//std::cout << "after ending render \n";
			return false;
		}
		return true;
	}

	void LevelCreationScene::renderBackgroundGrid(std::pair<VkCommandBuffer, uint8_t>& cmdIndexPair) {

		PipelineSystem::setCmdIndexPair(cmdIndexPair);
		PipelineSystem::at(Pipe_Grid2d)->bindPipeline();
		Simple2DPushConstantData push;
		push.scaleOffset = { pushScale, pushTrans };
		push.color.r = 0.f;
		push.color.g = 125.6f / 255.f;
		push.color.b = 90.f / 255.f;
		PipelineSystem::at(Pipe_Grid2d)->pushAndDraw(&push);
	}
}