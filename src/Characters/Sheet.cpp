#include "Sheet.h"

namespace EWE {
	Sheet::Sheet(EWEDevice& device) {

		skeleton = std::make_shared<SheetSkeleton>(device);

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1); // if issues, set this up first
	}
	Sheet::~Sheet() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
	}
	void Sheet::scare() {
		if (animState == SheetSkeleton::Anim_idle) {
			animFrame = 0;
			animState = SheetSkeleton::Anim_scared;
		}
	}
	void Sheet::flee() {
		animFrame = 0;
		animState = SheetSkeleton::Anim_walk;
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
			else {
				
				glm::vec3 targetDir = target - transform.translation;
				float targetDist = glm::length(targetDir);
				if (targetDist < 0.2f) {
					animFrame = 0;
					animState = SheetSkeleton::Anim_idle;
					return;
				}
				targetDir /= targetDist;
				transform.translation += targetDir / 62.5f;

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
	}
	void Sheet::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}