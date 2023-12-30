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

		if (SaveJSON::saveData.petFlags & SaveJSON::PetFlags::PF_Carrot) {
			carrotPet = std::make_unique<CarrotPet>(device, globalPool);
		}

	}
	Charmer::~Charmer() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
		skeleton.reset();
	}

	void Charmer::logicUpdate() {
		CharmerKeys polledKeys = inputHandler.pollInput();
		movement(polledKeys);

		if (carrotPet.get() != nullptr) {
			updateCarrotPet(polledKeys);
		}
		if (dynamic_cast<ForestLevel*>(currentLevel) != nullptr) {
			if (((ForestLevel*)currentLevel)->pickLog(transform.translation.x, transform.translation.z)) {

			}
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

		if (carrotPet.get() != nullptr) {
			carrotPet->transform.translation = transform.translation;
			//printf("forward dir : %.2f:%.2f \n", forwardDir.x, forwardDir.y);

			carrotPet->transform.translation.x += (forwardDir.x * 50.f);
			carrotPet->transform.translation.z += (forwardDir.y * 50.f);
			carrotPet->transform.rotation = transform.rotation;
			carrotPet->transform.rotation.y += glm::half_pi<float>();
			carrotPet->renderUpdate();
		}

	}

	void Charmer::movement(CharmerKeys polledKeys) {
		int horizontalMovement = polledKeys.leftPressed - polledKeys.rightPressed;
		int lateralMovement = polledKeys.backwardPressed - polledKeys.forwardPressed;
		glm::vec2 horizontalVelocity = glm::vec2{ 2.f / 250.f * horizontalMovement, 2.f / 250.f * lateralMovement };
		if ((horizontalMovement != 0) || (lateralMovement != 0)) {
			forwardDir = horizontalVelocity;
		}

		if ((horizontalMovement != 0) && (lateralMovement != 0)) {
			horizontalVelocity.x /= 1.41421356f;
			horizontalVelocity.y /= 1.41421356f;
			forwardDir /= 1.41421356f;

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
		//printf("tileFlag : %d \n", tileFlag);

		//printf("translation: %.2f:%.2f:%.2f \n", transform.translation.x, transform.translation.y, transform.translation.z);
		if ((uint16_t)tileFlag >= (uint16_t)TileFlag_exit1) {
			changeLevel = tileFlag - TileFlag_exit1;
			printf("setting changeLevel : %d \n", changeLevel);
		}
	}

	void Charmer::updateCarrotPet(CharmerKeys polledKeys) {
		carrotPet->animFrame++;
		//printf("carrot pet anim state:frame - %d:%d \n", carrotPet->animState, carrotPet->animFrame);

		switch (carrotPet->animState) {
		case CarrotSkeleton::Anim_chop: {
			if (carrotPet->animFrame == 100) {
				if (dynamic_cast<StartLevel*>(currentLevel) != nullptr) {
					printf("dynamic casted wasn't null \n");
					((StartLevel*)currentLevel)->chopTree({ transform.translation.x, transform.translation.z }, glm::normalize(forwardDir));
				}
				else if (dynamic_cast<ForestLevel*>(currentLevel) != nullptr) {
						printf("chopping in forest on charmer \n");
						((ForestLevel*)currentLevel)->chopTree(glm::vec2{ transform.translation.x, transform.translation.z }, glm::normalize(forwardDir));
				}
			}
			else if (carrotPet->animFrame >= 250) {
				carrotPet->animState = CarrotSkeleton::Anim_idle;
				carrotPet->animFrame = 0;
			}
			break;
		}
		case CarrotSkeleton::Anim_idle: {
			carrotPet->animFrame = carrotPet->animFrame % 100;
			if (polledKeys.actionPressed[0]) {
				printf("action 0 was pressed \n");

				carrotPet->animState = CarrotSkeleton::Anim_chop;
				carrotPet->animFrame = 0;

			}
			break;
		}
		case CarrotSkeleton::Anim_walk: {
			carrotPet->animFrame = carrotPet->animFrame % 100;
			if (polledKeys.actionPressed[0]) {
				printf("action 0 was pressed \n");

				carrotPet->animState = CarrotSkeleton::Anim_chop;
				carrotPet->animFrame = 0;

			}
			break;
		}
		default: {

			break;
		}
		}
	}
}