#include "ZeroPet.h"

namespace EWE {

	ZeroPet::ZeroPet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		skeleton = std::make_shared<ZeroSkeleton>(device);

		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
		bufferPointer->changeMaxActorCount(device, 1, globalPool); // if issues, set this up first

		//transform.scale = glm::vec3{ 0.5f };
	}
	ZeroPet::~ZeroPet() {
		SkinRenderSystem::removePushData(skeleton->getSkeletonID(), &pushData);
	}

	ZeroPet::ZeroPet(EWEDevice& device, std::shared_ptr<ZeroSkeleton> ZeroSkele) {
		skeleton = ZeroSkele;
		//transform.scale = glm::vec3{ 0.5f };
		SkinRenderSystem::setPushData(skeleton->getSkeletonID(), &pushData, static_cast<uint8_t>(sizeof(pushData)));
		bufferPointer = SkinRenderSystem::getSkinBuffer(skeleton->getSkeletonID());
	}


	void ZeroPet::logicUpdate() {

	}
	void ZeroPet::renderUpdate() {
		pushData.modelMatrix = transform.mat4();
		//printf("renderUpdate \n");

		bufferPointer->writeData(skeleton->getFinalBones((uint8_t)animState, animFrame));
		bufferPointer->flush();
	}
}