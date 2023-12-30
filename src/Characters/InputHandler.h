#pragma once
#include "EWEngine/Data/TransformInclude.h"

#include "EWEngine/SettingsJSON.h"

#include <map>
#include <vector>
#include <string>
#include <memory>


namespace EWE {
	struct CharmerKeyMappings {
		int action[4] = { GLFW_KEY_Q, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_I };
		int forward = GLFW_KEY_W;
		int backward = GLFW_KEY_S;
		int left = GLFW_KEY_A;
		int right = GLFW_KEY_D;
		//bool mouseLook = true;
	};

	struct CharmerKeys {
		std::pair<double, double> clickPosition = { -1.0, -1.0 };

		bool actionPressed[4] = { false, false, false, false };
		bool actionReleased[4] = { false, false, false, false };

		bool forwardPressed = false;
		bool backwardPressed = false;
		bool leftPressed = false;
		bool rightPressed = false;
	};


	class CharmerInput {
	protected:
		static CharmerInput* inputPtr;
	public:

		CharmerInput(GLFWwindow* wnd) : window{ wnd } {
			glfwGetCursorPos(wnd, &mousePosX, &mousePosY);
			inputPtr = this;
		}
		~CharmerInput() {
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
		static void giveCallbackReturns(GLFWmousebuttonfun mousecallback, GLFWkeyfun keycallback) {
			mousecallbackReturn = mousecallback;
			keycallbackReturn = keycallback;
		}
		static void returnCallbacks() {
			glfwSetKeyCallback(inputPtr->window, keycallbackReturn);
			glfwSetMouseButtonCallback(inputPtr->window, mousecallbackReturn);
		}

		static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void staticMouseCallback(GLFWwindow* window, int button, int action, int mods);

		CharmerKeys liveActions{};
		GLFWwindow* window;
		CharmerKeyMappings keyMaps;

		void returnFocus() {
			//glfwFocusWindow(window);
			//printf("player man input menu active to false \n");
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
	protected:
		static GLFWmousebuttonfun mousecallbackReturn;
		static GLFWkeyfun keycallbackReturn;

	};
}