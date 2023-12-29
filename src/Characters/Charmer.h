#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/graphics/EWE_Camera.h>

#include "CharmerSkeleton.h"
#include "InputHandler.h"
#include "../systems/Level.h"
#include "CarrotPet.h"

//#include <skeleton>

namespace EWE {
	class Charmer {
	private:
		static std::unique_ptr<CharmerSkeleton> skeleton;
	public:
		//std::unique_ptr<CharmerSkeleton> skeleton;
		//std::shared_ptr<EweObject> tool; //if wielding a tool?
		Charmer(EWEDevice& device, GLFWwindow* wnd, EWECamera& camera, std::shared_ptr<EWEDescriptorPool> globalPool);
		~Charmer();

		void giveInputFocus() {
			inputHandler.returnFocus();
		}
		bool wantsMenu() {
			if (inputHandler.menuActive) {
				inputHandler.menuActive = false;
				return true;
			}
			return false;
		}

		void logicUpdate();
		void renderUpdate();

		Level* currentLevel;

		int32_t getLevelChange() {
			if (changeLevel >= 0) {
				int32_t retBuffer = changeLevel;
				changeLevel = -1;
				return retBuffer;
			}
			return changeLevel;
		}

		void setTransform(TransformComponent const& transform) {
			this->transform = transform;
		}

	protected:
		int32_t changeLevel = -1;
		CharmerInput inputHandler;
		std::unique_ptr<CarrotPet> carrotPet;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};
		EWECamera& camera;

		uint32_t animFrame = 0;
		CharmerSkeleton::Charmer_Animations animState = CharmerSkeleton::Anim_idle;
	};
}

