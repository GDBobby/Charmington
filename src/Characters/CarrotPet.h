#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "CarrotSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class CarrotPet {
	protected:
		static std::shared_ptr<CarrotSkeleton> skeleton;

	public:
		CarrotPet(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool);

		void logicUpdate();
		void renderUpdate();

	protected:
		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};

		uint32_t animFrame = 0;
		CarrotSkeleton::Charmer_Animations animState = CarrotSkeleton::Anim_idle;
	};
}

