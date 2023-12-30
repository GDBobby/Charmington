#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "ZeroSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class Zero {
	protected:

	public:
		Zero(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool);
		~Zero();
		void logicUpdate();
		void renderUpdate();

		void setTransform(TransformComponent const& transformComponent) {
			this->transform = transformComponent;
		}
		bool tamed = false; //readonly from outside
		std::shared_ptr<ZeroSkeleton> skeleton;

		glm::vec3* charmerTranslation;

		void giveTreeLocations(std::vector<std::pair<bool, glm::vec3>>* trees) {
			treeData = trees;
		}
		void treeChopped(uint8_t treeID) {
			printf("sheet losing tree: %d \n", treeID);
			//appleLocations.push_back(treeID);
		}

	protected:
		std::vector<std::pair<bool, glm::vec3>>* treeData{ nullptr };
		int8_t hitSomething = -1;
		int8_t hidingSpots = 10;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};

		uint32_t animFrame = 0;
		ZeroSkeleton::Zero_Animations animState = ZeroSkeleton::Anim_idle;
	};
}

