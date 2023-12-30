#include "Charmer.h"

#define ZERO_FOLLOW_DISTANCE 125
#include <EWEngine/SoundEngine.h>
#include "../MusicEnum.h"

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
		if ((SaveJSON::saveData.petFlags & SaveJSON::PF_Zero) == SaveJSON::PF_Zero) {
			printf("loading zero into charmer \n");
			zeroPet = std::make_unique<ZeroPet>(device, globalPool);

			setZeroHistory();
		}

		logCount = SaveJSON::saveData.logCount;
		if (logCount > 9) {
			logCount = 9;
		}
	}
	Charmer::~Charmer() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
		skeleton.reset();
	}

	void Charmer::logicUpdate() {
		CharmerKeys polledKeys = inputHandler.pollInput();
		movement(polledKeys);
		if (zeroPet.get() != nullptr) {
			updateZeroPet(polledKeys);	
		}

		if (carrotPet.get() != nullptr) {
			updateCarrotPet(polledKeys);
		}
		if (dynamic_cast<ForestLevel*>(currentLevel) != nullptr) {
			updateForest(polledKeys);
		}
		if (dynamic_cast<ConnectorLevel*>(currentLevel) != nullptr) {
			updateConnector(polledKeys);
		}
		switch (animState) {
		case CharmerSkeleton::Anim_idle: {
			animFrame = (animFrame + 1) % 160;
			break;
		}
		case CharmerSkeleton::Anim_walk: {
			animFrame = (animFrame + 1) % 160;
			break;
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

		if (carrotPet.get() != nullptr) {
			carrotPet->transform.translation = transform.translation;
			//printf("forward dir : %.2f:%.2f \n", forwardDir.x, forwardDir.y);

			carrotPet->transform.translation.x += (forwardDir.x * 50.f);
			carrotPet->transform.translation.z += (forwardDir.y * 50.f);
			carrotPet->transform.rotation = transform.rotation;
			//carrotPet->transform.rotation.y += glm::quarter_pi<float>();
			carrotPet->renderUpdate();
		}
		if (zeroPet.get() != nullptr) {

			zeroPet->renderUpdate();
		}
	}

	void Charmer::movement(CharmerKeys polledKeys) {
		int horizontalMovement = polledKeys.leftPressed - polledKeys.rightPressed;
		int lateralMovement = polledKeys.backwardPressed - polledKeys.forwardPressed;
		glm::vec2 horizontalVelocity = glm::vec2{ 2.f / 250.f * horizontalMovement, 2.f / 250.f * lateralMovement };
		if ((horizontalMovement != 0) || (lateralMovement != 0)) {
			forwardDir = horizontalVelocity;
			if (zeroPet.get() != nullptr) {
				oldTransforms.push(transform);
			}
			if (carrotPet.get() != nullptr) {
				if (carrotPet->animState != CarrotSkeleton::Anim_walk) {
					carrotPet->animFrame = 0;
					carrotPet->animState = CarrotSkeleton::Anim_walk;
				}
			}
			
			if (animState != CharmerSkeleton::Anim_walk) {
				animFrame = 0;
				animState = CharmerSkeleton::Anim_walk;
			}
			
		}
		else {
			if (carrotPet.get() != nullptr) {
				if (carrotPet->animState == CarrotSkeleton::Anim_walk) {
					carrotPet->animFrame = 0;
					carrotPet->animState = CarrotSkeleton::Anim_idle;
				}
			}
			if (animState == CharmerSkeleton::Anim_walk) {
				animFrame = 0;
				animState = CharmerSkeleton::Anim_idle;
			}
		}

		if ((horizontalMovement != 0) && (lateralMovement != 0)) {
			horizontalVelocity.x /= 1.41421356f;
			horizontalVelocity.y /= 1.41421356f;
			forwardDir /= 1.41421356f;

			//right forward
			if (horizontalMovement == -1) {
				if (lateralMovement == -1) {
					transform.rotation.y = -glm::quarter_pi<float>() * 3.f;
				}
				else if (lateralMovement == 1) {
					transform.rotation.y = -glm::quarter_pi<float>();
				}
			}
			else if (horizontalMovement == 1) {
				//left
				if (lateralMovement == -1) {
					//forward
					transform.rotation.y = glm::quarter_pi<float>() * 3.f;
				}
				else if (lateralMovement == 1) {
					transform.rotation.y = glm::quarter_pi<float>();
				}
			}
		}
		else {
			if (lateralMovement == 1) {
				//going down
				transform.rotation.y = 0.f;
			}
			else if (lateralMovement == -1) {
				transform.rotation.y = glm::pi<float>();
				//forward
			}
			if (horizontalMovement == 1) {
				//left
				transform.rotation.y = glm::half_pi<float>();
			}
			else if (horizontalMovement == -1) {
				transform.rotation.y = -glm::half_pi<float>();
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
					//printf("pos hori clamp \n");
					//printf("before, after - %.5f:%.5f \n", transform.translation.z, std::ceil(transform.translation.z) - 1.001f);
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

			//printf("setting changeLevel : %d \n", changeLevel);
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
					if (((StartLevel*)currentLevel)->chopTree({ transform.translation.x, transform.translation.z }, glm::normalize(forwardDir))) {
						SoundEngine::getSoundEngineInstance()->playEffect(FX_chop);
					}
				}
				else if (dynamic_cast<ForestLevel*>(currentLevel) != nullptr) {
						printf("chopping in forest on charmer \n");
						if (((ForestLevel*)currentLevel)->chopTree(glm::vec2{ transform.translation.x, transform.translation.z }, glm::normalize(forwardDir))) {
							SoundEngine::getSoundEngineInstance()->playEffect(FX_chop);
						}
				}
				else if (dynamic_cast<SpookyForest*>(currentLevel) != nullptr) {
					printf("chopping tree in spooky forest \n");
					if (((SpookyForest*)currentLevel)->chopTree(glm::vec2{ transform.translation.x, transform.translation.z }, glm::normalize(forwardDir))) {
						SoundEngine::getSoundEngineInstance()->playEffect(FX_chop);
					}
				}
			}
			else if (carrotPet->animFrame >= 250) {
				carrotPet->animFrame = 0;
				carrotPet->animState = CarrotSkeleton::Anim_idle;
			}
			break;
		}
		case CarrotSkeleton::Anim_idle: {
			carrotPet->animFrame = carrotPet->animFrame % 400;
			if (polledKeys.actionPressed[0]) {
				printf("action 0 was pressed \n");

				carrotPet->animFrame = 0;
				carrotPet->animState = CarrotSkeleton::Anim_chop;

			}
			break;
		}
		case CarrotSkeleton::Anim_walk: {
			carrotPet->animFrame = carrotPet->animFrame % 200;
			if (polledKeys.actionPressed[0]) {
				printf("action 0 was pressed \n");

				carrotPet->animFrame = 0;
				carrotPet->animState = CarrotSkeleton::Anim_chop;

			}
			break;
		}
		default: {

			break;
		}
		}
	}
	void Charmer::tamedZero(EWEDevice& device, std::shared_ptr<ZeroSkeleton> zeroSkele) {
		zeroPet = std::make_unique<ZeroPet>(device, zeroSkele);


		setZeroHistory();
	}
	void Charmer::setTransform(TransformComponent const& transform) {
		this->transform = transform;
		if (zeroPet.get() != nullptr) {
			setZeroHistory();
		}
	}
	void Charmer::setZeroHistory() {
		zeroPet->transform = transform;
		zeroPet->transform.translation.x -= forwardDir.x * 100.f;
		zeroPet->transform.translation.z -= forwardDir.y * 100.f;

		while (!oldTransforms.empty()) {
			oldTransforms.pop();
		}
		while (oldTransforms.size() < ZERO_FOLLOW_DISTANCE) {
			oldTransforms.push(zeroPet->transform);
		}
	}
	void Charmer::updateZeroPet(CharmerKeys polledKeys) {
		if (oldTransforms.size() > ZERO_FOLLOW_DISTANCE) {
			zeroPet->animState = ZeroSkeleton::Anim_walk;
			zeroPet->animFrame++;
			if (zeroPet->animFrame >= 160) {
				zeroPet->animFrame = 0;
			}
			//printf("transform queue size : %d \n", oldTransforms.size());
			zeroPet->transform.translation = oldTransforms.front().translation;
			zeroPet->transform.rotation = oldTransforms.front().rotation;
			//zeroPet->transform.rotation.y += glm::quarter_pi<float>();
			oldTransforms.pop();
		}
		else {
			zeroPet->animFrame = 0;
			zeroPet->animState = ZeroSkeleton::Anim_idle;
		}
		if (polledKeys.actionPressed[2]) {
			//bark
			if (barkFrames == 0) {
				printf("bork \n");
				SoundEngine::getSoundEngineInstance()->playEffect(1);
				barkFrames = 1;
			}
		}
		if (barkFrames > 0) {
			barkFrames = (barkFrames + 1) % 350;
			//printf("barkFrames : %d \n", barkFrames);
		}

		if (barkFrames == 300) {
			if (dynamic_cast<ConnectorLevel*>(currentLevel) != nullptr) {
				((ConnectorLevel*)currentLevel)->bark(transform.translation.x, transform.translation.z);
			}
			if (dynamic_cast<SpookyForest*>(currentLevel) != nullptr) {
				((SpookyForest*)currentLevel)->bark(transform.translation.x, transform.translation.z);
			}
		}

	}
	void Charmer::updateForest(CharmerKeys polledKeys) {
		if (((ForestLevel*)currentLevel)->pickLog(transform.translation.x, transform.translation.z)) {
			logCount++;
			if (logCount > 9) {
				logCount = 9;
			}
			SaveJSON::saveData.logCount = logCount;
		}
		if (polledKeys.actionPressed[1] && (logCount > 0)) {
			printf("E was pressed? \n");
			logCount--;
			((ForestLevel*)currentLevel)->dropStick(transform.translation.x, transform.translation.z, glm::normalize(forwardDir));
		}
	}
	void Charmer::updateConnector(CharmerKeys polledKeys) {

		if (polledKeys.actionPressed[1] && (logCount > 0)) {
			printf("E was pressed? \n");
			if (((ConnectorLevel*)currentLevel)->scaredSheet) {
				logCount -= ((ConnectorLevel*)currentLevel)->dropPlank(transform.translation.x, transform.translation.z, glm::normalize(forwardDir));
			}
		}
	}
}