#pragma once

#include <EWEngine/Systems/Rendering/Skin/SkinRS.h>
#include <EWEngine/Graphics/PushConstants.h>

#include "ZeroSkeleton.h"
#include "../Systems/Level.h"

namespace EWE {
	class Zero {
	protected:

	public:
		Zero(EWEDevice& device);
		~Zero();
		void logicUpdate();
		void renderUpdate();

		void setTransform(TransformComponent const& transformComponent) {
			this->transform = transformComponent;
		}
		bool tamed = false; //readonly from outside
		std::shared_ptr<ZeroSkeleton> skeleton;

		glm::vec3* charmerTranslation;

		void treeChopped(uint8_t treeID) {
			printf("sheet losing tree: %d \n", treeID);
			//appleLocations.push_back(treeID);
		}
		void giveSticks(std::vector<Billboard>* sticks) {
			this->sticks = sticks;
		}

	protected:
		std::vector<Billboard>* sticks;
		uint8_t stickCount = 0;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};

		uint32_t animFrame = 0;
		ZeroSkeleton::Zero_Animations animState = ZeroSkeleton::Anim_idle;
	};
}

