#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "SheetSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class Sheet {
	protected:

	public:
		Sheet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool);
		~Sheet();
		void logicUpdate();
		void renderUpdate();

		void setTransform(TransformComponent const& transformComponent) {
			this->transform = transformComponent;
		}
		bool tamed = false; //readonly from outside
		std::shared_ptr<SheetSkeleton> skeleton;

		glm::vec3* charmerTranslation;

		void giveTreeLocations(std::vector<std::pair<bool, glm::vec3>>* trees) {
			treeData = trees;
		}
		void treeChopped(uint8_t treeID) {
			printf("sheet losing tree: %d \n", treeID);
			//appleLocations.push_back(treeID);
		}
		void scare();
		void flee();
		void GoTo(glm::vec3 target) {
			this->target = target;
			animFrame = 0;
			animState = SheetSkeleton::Anim_walk;
		}
		TransformComponent transform{};

		bool scaredAnim() {
			return animState == SheetSkeleton::Anim_scared;
		}
		bool escaped = false;
		bool guarding = false;
		SheetSkeleton::Sheet_Animations animState = SheetSkeleton::Anim_idle;
	protected:
		glm::vec3 target;
		std::vector<std::pair<bool, glm::vec3>>* treeData{nullptr};
		int8_t hitSomething = -1;
		int8_t hidingSpots = 10;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};

		uint32_t animFrame = 0;
	};
}

