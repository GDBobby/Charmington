#pragma once
#include "EWEngine/Data/TransformInclude.h"

#include "EWEngine/SettingsJSON.h"

#include <map>
#include <vector>
#include <string>
#include <memory>


namespace EWE {
	struct CharmerKeyMappings {
		int attack[4] = { GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R };
		int keyPickup = GLFW_KEY_F;
		int keyInventory = GLFW_KEY_I;
		//bool mouseLook = true;
	};

	struct CharmerKeys {
		std::pair<double, double> clickPosition = { -1.0, -1.0 };
		//bool attackPressed[4] = { false, false, false, false };
		//bool attackReleased[4] = { false, false, false, false };

		bool pickupPressed = false;
		bool pickupReleased = false;
		bool inventoryPressed = false;
		bool inventoryReleased = false;
	};


	class CharmerInput {
	protected:
		static std::shared_ptr<CharmerInput> SingleInstance;
	public:
		static std::shared_ptr<CharmerInput>& GetInputHandlerInstance() {
			return SingleInstance;
		}
		static void InitializeInputHandler(GLFWwindow* wnd) {
			static bool initialized = false;
			if (!initialized) {
				SingleInstance = std::make_shared<CharmerInput>(wnd);
				initialized = true;
			}
			else {
				printf("trying to init input handler a second time? \n");
			}
		}

		CharmerInput(GLFWwindow* wnd) : window{ wnd } {
			glfwGetCursorPos(wnd, &mousePosX, &mousePosY);
			inputPtr = this;
		}
		~CharmerInput() {
			//printf("input handler deconstruciton \n");
			inputPtr = nullptr;
		}
		/*
		void init(GLFWwindow* wnd) {
			//printf("inputHandler init \n");
			window = wnd;
			inputPtr = this;
		}
		*/

		//static InputHandler* inputPointer;
		static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void staticMouseCallback(GLFWwindow* window, int button, int action, int mods);

		CharmerKeys liveActions{};
		GLFWwindow* window;
		CharmerKeyMappings keyMaps;
		static CharmerInput* inputPtr;

		void returnFocus() {
			//glfwFocusWindow(window);
			printf("player man input menu active to false \n");
			menuActive = false;
			glfwGetCursorPos(window, &mousePosX, &mousePosY);
			glfwSetKeyCallback(window, staticKeyCallback);
			glfwSetMouseButtonCallback(window, staticMouseCallback);
		}

		bool menuActive = true;

		uint16_t currentFrame = 0;
		bool actionBufferTracker = false;
		CharmerKeys pollInput();

		double mousePosX, mousePosY;

		void Reset() {
			liveActions = CharmerKeys{};
		}

	};
}