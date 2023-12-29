#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "CarrotSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class CarrotPet {
	protected:

	public:
		CarrotPet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool);
		CarrotPet(EWEDevice& device, std::shared_ptr<CarrotSkeleton> carrotSkele);
		~CarrotPet();

		void logicUpdate();
		void renderUpdate();

		std::shared_ptr<CarrotSkeleton> skeleton;


		TransformComponent transform{};

	protected:
		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};

		uint32_t animFrame = 0;
		CarrotSkeleton::Charmer_Animations animState = CarrotSkeleton::Anim_idle;
	};
}

