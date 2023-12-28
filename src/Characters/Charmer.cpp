#include "Charmer.h"

namespace EWE {
	Charmer::Charmer(GLFWwindow* wnd, EWECamera& camera) : inputHandler{ wnd }, camera{ camera } {

	}

	void Charmer::logicUpdate() {
		CharmerKeys polledKeys = inputHandler.pollInput();

		int lateralMovement = polledKeys.forwardPressed + polledKeys.backwardPressed;
		
		if (lateralMovement != 0) {
			//if movable
			transform.translation.x += 2.f / 250.f * lateralMovement;
		}
		int horizontalMovement = polledKeys.leftPressed + polledKeys.rightPressed;
		if (horizontalMovement != 0) {
			transform.translation.z += 2.f / 250.f * horizontalMovement;
		}


	}

	void Charmer::renderUpdate() {
		//update tool position
		//update pet position
		//update animation
		glm::vec3 cameraPos = transform.translation + glm::vec3{ 0.f, 10.f, 5.f };
		glm::vec3 target = transform.translation;
		camera.updateViewData(cameraPos, target);

	}
}