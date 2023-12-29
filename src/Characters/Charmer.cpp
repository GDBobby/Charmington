#include "Charmer.h"

namespace EWE {
	std::unique_ptr<CharmerSkeleton> Charmer::skeleton{nullptr};

	void inline clampCeil(float& value) {
		value = std::ceil(value * 2.f) / 2.f - .001f;
	}
	void inline clampFloor(float& value) {
		value = std::floor(value * 2.f) / 2.f + .001f;
	}

	Charmer::Charmer(EWEDevice& device, GLFWwindow* wnd, EWECamera& camera, std::shared_ptr<EWEDescriptorPool> globalPool) : inputHandler{ wnd }, camera{ camera } {
		if (skeleton.get() == nullptr) {
			printf("current directory? : %s \n", std::filesystem::current_path().string().c_str());
			skeleton = std::make_unique<CharmerSkeleton>(device);
		}
		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first

		transform.rotation.y = -glm::half_pi<float>();
	}
	Charmer::~Charmer() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
		skeleton.reset();
	}

	void Charmer::logicUpdate() {
		CharmerKeys polledKeys = inputHandler.pollInput();
		
		int horizontalMovement = polledKeys.leftPressed - polledKeys.rightPressed;
		int lateralMovement = polledKeys.backwardPressed - polledKeys.forwardPressed;
		glm::vec2 horizontalVelocity = glm::vec2{ 2.f / 250.f * horizontalMovement, 2.f / 250.f * lateralMovement };


		if ((horizontalMovement != 0) && (lateralMovement != 0)) {
			horizontalVelocity.x /= 1.41421356f;
			horizontalVelocity.y /= 1.41421356f;

			//right forward
			if (horizontalMovement == -1) {
				if (lateralMovement == -1) {
					transform.rotation.y = glm::quarter_pi<float>() * 3.f;
				}
				else if (lateralMovement == 1) {
					transform.rotation.y = glm::quarter_pi<float>() * -3.f;
				}
			}
			else if (horizontalMovement == 1) {
				//left
				if (lateralMovement == -1) {
					//forward
					transform.rotation.y = glm::quarter_pi<float>();
				}
				else if (lateralMovement == 1) {
					transform.rotation.y = -glm::quarter_pi<float>();
				}
			}
		}
		else {
			if (lateralMovement == 1) {
				//going down
				transform.rotation.y = -glm::half_pi<float>();
			}
			else if (lateralMovement == -1) {
				transform.rotation.y = glm::half_pi<float>();
				//forward
			}
			if (horizontalMovement == 1) {
				//left
				transform.rotation.y = 0.f;
			}
			else if (horizontalMovement == -1) {
				transform.rotation.y = glm::pi<float>();
				//right
			}
		}
		//forward left, forward right
		//back left, back right
		//left, right, forward, back


		if (currentLevel->tileAt(transform.translation.x + horizontalVelocity.x, transform.translation.z + horizontalVelocity.y) != TileFlag_solid) {
			transform.translation.x += horizontalVelocity.x;
			transform.translation.z += horizontalVelocity.y;
		}
		else {
			if (currentLevel->tileAt(transform.translation.x + horizontalVelocity.x, transform.translation.z) != TileFlag_solid) {
				transform.translation.x += horizontalVelocity.x;
				if (horizontalVelocity.y > 0.f) {
					printf("pos hori clamp \n");
					printf("before, after - %.5f:%.5f \n", transform.translation.z, std::ceil(transform.translation.z) - 1.001f);
					clampCeil(transform.translation.z);
				}
				else if (horizontalVelocity.y < 0.f) {
					clampFloor(transform.translation.z);
				}
			}
			else if (currentLevel->tileAt(transform.translation.x, transform.translation.z + horizontalVelocity.y) != TileFlag_solid) {
				transform.translation.z += horizontalVelocity.y;
				if (horizontalVelocity.x > 0.f) {
					clampCeil(transform.translation.x);
				}
				else if (horizontalVelocity.x < 0.f) {
					clampFloor(transform.translation.x);// = std::floor(transform.translation.x * 2.f) / 2.f + .001f;
				}
			}
			else {
				if (horizontalVelocity.x > 0.f) {
					clampCeil(transform.translation.x);
				}
				else if (horizontalVelocity.x < 0.f) {
					clampFloor(transform.translation.x);
				}
				if (horizontalVelocity.y > 0.f) {
					clampCeil(transform.translation.z);
				}
				else if (horizontalVelocity.y < 0.f) {
					clampFloor(transform.translation.z);
				}
			}
		}
		TileFlag tileFlag = currentLevel->tileAt(transform.translation.x, transform.translation.z);
		printf("translation: %.2f:%.2f:%.2f \n", transform.translation.x, transform.translation.y, transform.translation.z);
		if ((uint16_t)tileFlag >= (uint16_t)TileFlag_exit1) {
			changeLevel = tileFlag - TileFlag_exit1;
			printf("setting changeLevel : %d \n", changeLevel);
		}

	}

	void Charmer::renderUpdate() {
		//update tool position
		//update pet position
		//update animation
		glm::vec3 cameraPos = transform.translation + glm::vec3{ 0.f, 6.f, 3.f };
		glm::vec3 target = transform.translation;
		camera.updateViewData(cameraPos, target);

		pushData.modelMatrix = transform.mat4();

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
		animFrame = (animFrame + 1) % 80;

	}
}