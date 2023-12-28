#include "InputHandler.h"

namespace EWE {
	CharmerKeys CharmerInput::pollInput() {
		CharmerKeys returnBuffer = liveActions;

        returnBuffer.forwardPressed = glfwGetKey(window, keyMaps.forward);
        returnBuffer.leftPressed = glfwGetKey(window, keyMaps.left);
        returnBuffer.backwardPressed = glfwGetKey(window, keyMaps.backward);
        returnBuffer.rightPressed = glfwGetKey(window, keyMaps.right);


		liveActions = CharmerKeys{};
		return returnBuffer;
	}

    CharmerInput* CharmerInput::inputPtr{nullptr};
    GLFWmousebuttonfun CharmerInput::mousecallbackReturn;
    GLFWkeyfun CharmerInput::keycallbackReturn;



	void CharmerInput::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == 1) {
            //std::string tempString = getMouse(button);
            for (int i = 0; i < 4; i++) {
                if (key == inputPtr->keyMaps.action[i]) {
                    inputPtr->liveActions.actionPressed[i] = true;
                }
            }

            //std::cout << "mouse callback : " << button << std::endl;
            switch (key) {
                case GLFW_KEY_W:
					inputPtr->liveActions.forwardPressed = true;
					break;
                case GLFW_KEY_S:
                    inputPtr->liveActions.backwardPressed = true;
                    break;
                case GLFW_KEY_A:
                    inputPtr->liveActions.leftPressed = true;
                    break;
                case GLFW_KEY_D:
                    inputPtr->liveActions.rightPressed = true;
					break;
                case GLFW_KEY_ESCAPE: {
                    if (inputPtr->menuActive) {
                        inputPtr->menuActive = false;
                        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        inputPtr->returnFocus();
                    }
                    else {
                        printf("opening menu from input handler \n");
                        inputPtr->menuActive = true;
                        //returnCallbacks();
                        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                    break;
                }
                default:
                    break;
            }
            
        }
        if (action == 0) {
            for (int i = 0; i < 4; i++) {
                if (key == inputPtr->keyMaps.action[i]) {
                    inputPtr->liveActions.actionReleased[i] = true;
                }
            }
            switch (key) {
            case GLFW_KEY_W:
                inputPtr->liveActions.forwardPressed = false;
                break;
            case GLFW_KEY_S:
                inputPtr->liveActions.backwardPressed = false;
                break;
            case GLFW_KEY_A:
                inputPtr->liveActions.leftPressed = false;
                break;
            case GLFW_KEY_D:
                inputPtr->liveActions.rightPressed = false;
                break;
            default:
                break;
            }
        }


	}
	void CharmerInput::staticMouseCallback(GLFWwindow* window, int button, int action, int mods) {

	}
}