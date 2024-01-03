#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>

#include "ZeroSkeleton.h"
#include "../systems/Level.h"

namespace EWE {
	class ZeroPet {
	protected:

	public:
		ZeroPet(EWEDevice& device);
		ZeroPet(EWEDevice& device, std::shared_ptr<ZeroSkeleton> ZeroSkele);
		~ZeroPet();

		void logicUpdate();
		void renderUpdate();

		std::shared_ptr<ZeroSkeleton> skeleton;


		TransformComponent transform{};

		uint32_t animFrame = 0;
		ZeroSkeleton::Zero_Animations animState = ZeroSkeleton::Anim_idle;
	protected:
		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};

	};
}

