#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/graphics/EWE_Camera.h>

#include "CharmerSkeleton.h"
#include "InputHandler.h"
//#include <skeleton>

namespace EWE {
	class Charmer {
	private:
		static std::unique_ptr<CharmerSkeleton> skeleton;
	public:
		//std::unique_ptr<CharmerSkeleton> skeleton;
		//std::shared_ptr<EweObject> tool; //if wielding a tool?
		Charmer(GLFWwindow* wnd, EWECamera& camera);

		CharmerInput inputHandler;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData;
		TransformComponent transform;
		EWECamera& camera;

		void logicUpdate();
		void renderUpdate();
	};
}

