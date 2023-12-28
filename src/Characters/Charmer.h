#pragma once

#include <EWEngine/systems/SkinRendering/SkinRenderSystem.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/graphics/EWE_Camera.h>

#include "CharmerSkeleton.h"
#include "InputHandler.h"
#include "../systems/Level.h"

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
		glm::vec3 const& getTranslation() {
			return transform.translation;
		}

		Level* currentLevel;
	protected:
		CharmerInput inputHandler;

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};
		EWECamera& camera;

		uint32_t animFrame = 0;
		CharmerSkeleton::Charmer_Animations animState = CharmerSkeleton::Anim_idle;
	};
}

