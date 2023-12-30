#include "Sheet.h"

namespace EWE {
	Sheet::Sheet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {

		skeleton = std::make_shared<SheetSkeleton>(device);

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first
	}
	Sheet::~Sheet() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
	}
	void Sheet::scare() {
		if (animState == SheetSkeleton::Anim_idle) {
			animState = SheetSkeleton::Anim_scared;
			animFrame = 0;
		}
	}
	void Sheet::flee() {
		animState = SheetSkeleton::Anim_walk;
		animFrame = 0;
	}

	void Sheet::logicUpdate() {


		switch (animState) {
		case SheetSkeleton::Anim_idle: {
			animFrame = (animFrame + 1) % 200;
			break;
		}
		case SheetSkeleton::Anim_walk: {
			animFrame++;
			if (animFrame >= 249) {
				animFrame = 50;
			}
			if (guarding) {
				transform.translation.z -= 1.f / 250.f;
				transform.rotation.y = glm::half_pi<float>();
				if (transform.translation.z < -26.f) {
					escaped = true;
				}
			}
			break;
		}
		case SheetSkeleton::Anim_scared: {
			animFrame++;
			if (animFrame >= 260) {
				animFrame = 0;
				animState = SheetSkeleton::Anim_idle;
			}
			break;
		}
		default:
			break;
		}
		/*
		if (animState != SheetSkeleton::Anim_chop) {
			if (appleLocations.size() > 0) {
				printf("apple locations size? : %lu \n", appleLocations.size());
				float shortestDist = 10000000.f; //big value
				std::pair<uint8_t, glm::vec3>* closestApple = nullptr;
				glm::vec2 appleDirection;

				for (auto& apple : appleLocations) {
					//find closest apple
					appleDirection.x = apple.second.x - transform.translation.x;
					appleDirection.y = apple.second.z - transform.translation.z;
					float currentDist = appleDirection.x * appleDirection.x + appleDirection.y * appleDirection.y;

					if (currentDist < shortestDist) {
						shortestDist = currentDist;
						closestApple = &apple;
					}
				}
				if (shortestDist < 0.25f) {
					//eat the apple
					appleAteCount++;
					if (appleAteCount >= 3) {
						tamed = true;
					}
					ateAppleID = closestApple->first;
					for (int i = 0; i < appleLocations.size(); i++) {
						if (appleLocations[i].first == closestApple->first) {
							appleLocations.erase(appleLocations.begin() + i);
							break;
						}
					}
				}
				else {

					appleDirection = glm::normalize(appleDirection);
					transform.translation.x += appleDirection.x / 250.f;
					transform.translation.z += appleDirection.y / 250.f;

					transform.rotation.y = glm::acos(glm::dot(appleDirection, glm::vec2(0.f, 1.f)));
					transform.rotation.y *= (1.f - 2.f * (appleDirection.x < 0.f));
				}


			}
			else {

				glm::vec2 charmerDirection = { charmerTranslation->x - transform.translation.x, charmerTranslation->z - transform.translation.z };
				float charmerDistanceSquared = charmerDirection.x * charmerDirection.x + charmerDirection.y * charmerDirection.y;
				if (charmerDistanceSquared < 1.f) {
					animState = CarrotSkeleton::Anim_chop;
					animFrame = 0;
				}
				else if (charmerDistanceSquared < 25.f) {
					charmerDirection = glm::normalize(charmerDirection);

					transform.translation.x += charmerDirection.x / 250.f;
					transform.translation.z += charmerDirection.y / 250.f;

					transform.rotation.y = glm::acos(glm::dot(charmerDirection, glm::vec2(0.f, 1.f)));
					transform.rotation.y *= (1.f - 2.f * (charmerDirection.x < 0.f));

					if (animState != CarrotSkeleton::Anim_walk) {
						animState = CarrotSkeleton::Anim_walk;
						animFrame = 0;
					}

				}
			}
		}
		else {
			animFrame++;
			if (animFrame < 100) {
				glm::vec2 charmerDirection = { charmerTranslation->x - transform.translation.x, charmerTranslation->z - transform.translation.z };
				charmerDirection = glm::normalize(charmerDirection);

				transform.rotation.y = glm::acos(glm::dot(charmerDirection, glm::vec2(0.f, 1.f)));
				transform.rotation.y *= (1.f - 2.f * (charmerDirection.x < 0.f));
			}

			if (animFrame == 100) {
				glm::vec2 charmerDirection = { charmerTranslation->x - transform.translation.x, charmerTranslation->z - transform.translation.z };
				float charmerDistanceSquared = charmerDirection.x * charmerDirection.x + charmerDirection.y * charmerDirection.y;

				if (charmerDistanceSquared < 1.f) {
					printf("hit the charmer \n");
					hitSomething = 0;
				}
				else {
					printf("checking tree for apples : %d \n", treeLocations.size());
					for (int i = 0; i < treeLocations.size(); i++) {
						float treeDistance = (treeLocations[i].x - transform.translation.x) * (treeLocations[i].x - transform.translation.x) + (treeLocations[i].z - transform.translation.z) * (treeLocations[i].z - transform.translation.z);
						printf("treeDistance : %.2f \n", treeDistance);
						if (treeDistance < 2.25f) {
							hitSomething = i + 1;
						}
					}
				}
			}
			else if (animFrame == 250) {
				animState = CarrotSkeleton::Anim_idle;
				animFrame = 0;
			}
		}
		*/
	}
	void Sheet::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}