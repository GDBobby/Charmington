#pragma once

#include <EWEngine/Systems/Rendering/Skin/SkinRS.h>
#include <EWEngine/Graphics/PushConstants.h>

#include "CarrotSkeleton.h"
#include "../Systems/Level.h"

namespace EWE {
	class CarrotPet {
	protected:

	public:
		CarrotPet(EWEDevice& device);
		CarrotPet(EWEDevice& device, std::shared_ptr<CarrotSkeleton> carrotSkele);
		~CarrotPet();

		void logicUpdate();
		void renderUpdate();

		std::shared_ptr<CarrotSkeleton> skeleton;


		TransformComponent transform{};

		uint32_t animFrame = 0;
		CarrotSkeleton::Carrot_Animations animState = CarrotSkeleton::Anim_idle;
	protected:
		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};

	};
}

