#include "Carrot.h"

#include <EWEngine/Sound_Engine.h>

#include "../MusicEnum.h"

namespace EWE {

	Carrot::Carrot(EWEDevice& device) {

		skeleton = std::make_shared<CarrotSkeleton>(device);
		

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1); // if issues, set this up first
	}
	Carrot::~Carrot() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
		SoundEngine::getSoundEngineInstance()->stopEfect(FX_steppage);
	}


	void Carrot::logicUpdate() {


		switch (animState) {
			case CarrotSkeleton::Anim_idle: {
				animFrame = (animFrame + 1) % 400;
				break;
			}
			case CarrotSkeleton::Anim_walk: {
				animFrame = (animFrame + 1) % 200;
				break;
			}
			case CarrotSkeleton::Anim_eat: {
				animFrame++;
				if (animFrame == 200) {
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
				if (animFrame >= 249) {
					animFrame = 0;
					animState = CarrotSkeleton::Anim_walk;
				}
				return;
				break;
			}
			default:
				break;
		}

		if (animState != CarrotSkeleton::Anim_chop) {
			if (appleLocations.size() > 0) {

				if (animState != CarrotSkeleton::Anim_walk) {
					SoundEngine::getSoundEngineInstance()->stopEfect(FX_steppage);
					SoundEngine::getSoundEngineInstance()->playEffect(FX_steppage, true);
					animFrame = 0;
					animState = CarrotSkeleton::Anim_walk;
				}

				printf("apple locations size? : %lu \n", appleLocations.size());
				float shortestDist = 10000000.f; //big value
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
					animFrame = 0;
					animState = CarrotSkeleton::Anim_eat;
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
					SoundEngine::getSoundEngineInstance()->stopEfect(FX_steppage);
					animFrame = 0;
					animState = CarrotSkeleton::Anim_chop;
				}
				else if (charmerDistanceSquared < 25.f) {
					charmerDirection = glm::normalize(charmerDirection);

					transform.translation.x += charmerDirection.x / 250.f;
					transform.translation.z += charmerDirection.y / 250.f;

					transform.rotation.y = glm::acos(glm::dot(charmerDirection, glm::vec2(0.f, 1.f)));
					transform.rotation.y *= (1.f - 2.f * (charmerDirection.x < 0.f));

					if (animState != CarrotSkeleton::Anim_walk) {
						SoundEngine::getSoundEngineInstance()->stopEfect(FX_steppage);
						SoundEngine::getSoundEngineInstance()->playEffect(FX_steppage, true);
						animFrame = 0;
						animState = CarrotSkeleton::Anim_walk;
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
			if (animFrame == 80) {
				SoundEngine::getSoundEngineInstance()->playEffect(FX_hya);
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
							SoundEngine::getSoundEngineInstance()->playEffect(FX_chop);
							hitSomething = i + 1;
						}
					}
				}
			}
			else if (animFrame == 250) {
				animFrame = 0;
				animState = CarrotSkeleton::Anim_idle;
			}
		}

	}
	void Carrot::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}