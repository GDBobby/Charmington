#include "Zero.h"

#include <EWEngine/SoundEngine.h>
#include "../MusicEnum.h"

namespace EWE {
	Zero::Zero(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {

		skeleton = std::make_shared<ZeroSkeleton>(device);

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first
	}
	Zero::~Zero() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
	}

	void Zero::logicUpdate() {

		glm::vec2 charmerDiff;
		charmerDiff.x = charmerTranslation->x - transform.translation.x;
		charmerDiff.y = charmerTranslation->z - transform.translation.z;
		float charmerDistSqd = charmerDiff.x * charmerDiff.x + charmerDiff.y * charmerDiff.y;

		switch (animState) {
		case ZeroSkeleton::Anim_idle: {
			animFrame = (animFrame + 1) % 879;
			bool visibleStick = false;
			for (auto& stick : *sticks) {
				if (!stick.drawable) {
					continue;
				}
				visibleStick = true;
			}
			if (visibleStick) {
				animFrame = 0;
				animState = ZeroSkeleton::Anim_walk;
			}
			if (charmerDistSqd > 9.f) {
				animFrame = 0;
				animState = ZeroSkeleton::Anim_walk;
				SoundEngine::getSoundEngineInstance()->playEffect(FX_atten);
			}

			break;
		}
		case ZeroSkeleton::Anim_walk: {
			animFrame++;
			if (animFrame >= 160) {
				animFrame = 0;
			}
			if (sticks->size() == 0){
				if (charmerDistSqd < 4.f) {
					animFrame = 0;
					animState = ZeroSkeleton::Anim_idle;
					transform.rotation.y = 0.f;
				}
				else {
					charmerDiff = glm::normalize(charmerDiff);
					transform.translation.x += charmerDiff.x / 250.f;
					transform.translation.z += charmerDiff.y / 250.f;

					transform.rotation.y = glm::acos(glm::dot(charmerDiff, glm::vec2(0.f, 1.f)));
					transform.rotation.y *= (1.f - 2.f * (charmerDiff.x < 0.f));
				}

			}
			else {
				float shortestDist = 1000000.f;
				Billboard* closestStick = nullptr;
				glm::vec2 closestDirection;
				bool visibleStick = false;
				for (auto& stick : *sticks) {
					if (!stick.drawable) {
						continue;
					}
					visibleStick = true;
					closestDirection.x = stick.translation.x - transform.translation.x;
					closestDirection.y = stick.translation.z - transform.translation.z;
					float currentDist = closestDirection.x * closestDirection.x + closestDirection.y * closestDirection.y;
					if (currentDist < shortestDist) {
						shortestDist = currentDist;
						closestStick = &stick;
					}
				}
				if (!visibleStick) {
					if (charmerDistSqd < 4.f) {
						animFrame = 0;
						animState = ZeroSkeleton::Anim_idle;
						transform.rotation.y = 0.f;
					}
					else {
						charmerDiff = glm::normalize(charmerDiff);
						transform.translation.x += charmerDiff.x / 250.f;
						transform.translation.z += charmerDiff.y / 250.f;

						transform.rotation.y = glm::acos(glm::dot(charmerDiff, glm::vec2(0.f, 1.f)));
						transform.rotation.y *= (1.f - 2.f * (charmerDiff.x < 0.f));
					}
					return;
				}
				if (shortestDist < 0.25f) {
					printf("Eating stick : %d \n", stickCount);
					closestStick->drawable = false;
					stickCount++;
					if (stickCount == 5) {
						printf("zero tamed \n");
						tamed = true;
					}
				}
				closestDirection = glm::normalize(closestDirection);
				transform.translation.x += closestDirection.x / 250.f;
				transform.translation.z += closestDirection.y / 250.f;

				transform.rotation.y = glm::acos(glm::dot(closestDirection, glm::vec2(0.f, 1.f)));
				transform.rotation.y *= (1.f - 2.f * (closestDirection.x < 0.f));
			}
			break;
		}
		default:
			break;
		}
	}
	void Zero::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}