#pragma once
#include <EWEngine/EightWindsEngine.h>
#include <EWEngine/Scene.h>
#include <EWEngine/imgui/imGuiHandler.h>
#include "../GUI/imGUI/LevelCreationIMGUI.h"

namespace EWE {
	class LevelCreationScene : public Scene {
	private:
		static LevelCreationScene* lcPtr;

		EightWindsEngine& ewEngine;
		MenuManager& menuManager;
		std::shared_ptr<SoundEngine> soundEngine;

	public:
		LevelCreationScene(EightWindsEngine& ewEngine);
		~LevelCreationScene();

		void load() override;
		void entry() override;
		void exit() override;
		bool render(double dt) override;

		void giveGLFWCallbackReturns(GLFWmousebuttonfun mouseReturnFunction, GLFWkeyfun keyReturnFunction) {
			this->mouseReturnFunction = mouseReturnFunction;
			this->keyReturnFunction = keyReturnFunction;
		}

	protected:
		std::unique_ptr<ImGUIHandler> imguiHandler{nullptr};

		bool show_demo_window = true;
		bool logicActive = false;
		void logicThreadFunction();
		std::unique_ptr<std::thread> logicThread;
		LevelCreationIMGUI levelCreationIMGUI;

		void renderBackgroundGrid(std::pair<VkCommandBuffer, uint8_t>& cmdIndexPair);
		glm::vec2 pushScale;
		glm::vec2 pushTrans;

		static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		GLFWmousebuttonfun mouseReturnFunction;
		GLFWkeyfun keyReturnFunction;

	};
}