#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "CarrotSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class Carrot {
	protected:

	public:
		Carrot(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool);
		~Carrot();
		void logicUpdate();
		void renderUpdate();

		void setTransform(TransformComponent const& transformComponent) {
			this->transform = transformComponent;
		}
		bool tamed = false;
		std::shared_ptr<CarrotSkeleton> skeleton;

		glm::vec3* charmerTranslation;

		int8_t checkHit() {
			if (hitSomething >= 0) {
				int8_t buffer = hitSomething;
				hitSomething = -1;
				return buffer;
			}
			return -1;
		}

		void giveTreeLocations(std::vector<glm::vec3> trees) {
			treeLocations = trees;
		}
		void appleDropped(std::pair<uint8_t,glm::vec3> appleInfo) {
			printf("carrot pushing apple data : %d \n", appleInfo.first);
			appleLocations.push_back(appleInfo);
		}
		int8_t ateApple() {
			if (ateAppleID > -1) {
				int8_t retBuffer = ateAppleID;
				printf("carrot ate an apple : %d \n", ateAppleID);
				ateAppleID = -1;
				return retBuffer;
			}
			return -1;
		}

	protected:
		std::vector<glm::vec3> treeLocations{};
		std::vector<std::pair<uint8_t, glm::vec3>> appleLocations{};
		int8_t hitSomething = -1;
		int8_t ateAppleID = -1;

		uint8_t appleAteCount = 0;

		std::pair<uint8_t, glm::vec3>* closestApple = nullptr;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};

		uint32_t animFrame = 0;
		CarrotSkeleton::Carrot_Animations animState = CarrotSkeleton::Anim_idle;
	};
}

