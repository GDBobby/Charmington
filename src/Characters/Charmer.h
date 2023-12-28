#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>
//#include <skeleton>

namespace EWE {
	class Charmer {
		//std::unique_ptr<CharmerSkeleton> skeleton;
		//std::shared_ptr<EweObject> tool; //if wielding a tool?

		std::shared_ptr<InputHandler> inputHandler{};

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData;

		void logicUpdate();
		void renderUpdate();
	};
}

