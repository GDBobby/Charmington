#include "CarrotPet.h"

namespace EWE {

	CarrotPet::CarrotPet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		skeleton = std::make_shared<CarrotSkeleton>(device);

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first

		transform.scale = glm::vec3{ 0.5f };
	}
	CarrotPet::~CarrotPet() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
	}

	CarrotPet::CarrotPet(EWEDevice& device, std::shared_ptr<CarrotSkeleton> carrotSkele) {
		skeleton = carrotSkele;
		transform.scale = glm::vec3{ 0.5f };
		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
	}


	void CarrotPet::logicUpdate() {

	}
	void CarrotPet::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}