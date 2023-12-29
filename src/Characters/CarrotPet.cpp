#include "CarrotPet.h"

namespace EWE {
	std::shared_ptr<CarrotSkeleton> CarrotPet::skeleton{ nullptr };

	CarrotPet::CarrotPet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {

		if (skeleton.get() == nullptr) {

			skeleton = std::make_shared<CarrotSkeleton>(device);
		}

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first
	}


	void CarrotPet::logicUpdate() {

	}
	void CarrotPet::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
		animFrame = (animFrame + 1) % 100;
	}
}