#pragma once

#include <EWEngine/Systems/Rendering/Skin/SkinRS.h>
#include <EWEngine/Graphics/PushConstants.h>
#include <EWEngine/Graphics/Camera.h>

#define RENDER_TRANSLATION true
#if RENDER_TRANSLATION
#include <EWEngine/Graphics/TextOverlay.h>
#include <format>
#endif

#include "CharmerSkeleton.h"
#include "InputHandler.h"

#include "../Systems/Level.h"
#include "../Levels/StartArea.h"
#include "../Levels/ForestLevel.h"
#include "../Levels/ConnectorLevel.h"
#include "../Levels/SpookyForest.h"


#include "CarrotPet.h"
#include "ZeroPet.h"

#include <queue>

//#include <skeleton>

namespace EWE {
	class Charmer {
	private:
		static std::unique_ptr<CharmerSkeleton> skeleton;
	public:
		//std::unique_ptr<CharmerSkeleton> skeleton;
		//std::shared_ptr<EweObject> tool; //if wielding a tool?
		Charmer(EWEDevice& device, GLFWwindow* wnd, EWECamera& camera);
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

		void setTransform(TransformComponent const& transform);
		void tamedCarrot(EWEDevice& device, std::shared_ptr<CarrotSkeleton> carrotSkele) {
			carrotPet = std::make_unique<CarrotPet>(device, carrotSkele);
		}
		void tamedZero(EWEDevice& device, std::shared_ptr<ZeroSkeleton> zeroSkele);

		glm::vec3* getTranslationPtr() {
			return &transform.translation;
		}
#if RENDER_TRANSLATION
		void drawTranslation() {
			constexpr const char* formatString = "Translation : {:.2f}:{:.2f}:{:.2f}";
			translationRender.string = std::format(formatString, transform.translation.x, transform.translation.y, transform.translation.z);

			TextOverlay::staticAddText(translationRender);
		}
#endif
		uint8_t logCount;
	protected:
#if RENDER_TRANSLATION
		TextStruct translationRender{ "translation : ", 1920, 1000, TA_right, 1.f };
#endif
		int32_t changeLevel = -1;
		CharmerInput inputHandler;
		std::unique_ptr<CarrotPet> carrotPet{nullptr};
		std::unique_ptr<ZeroPet> zeroPet{ nullptr };

		SkinBufferHandler* bufferPointer{ nullptr };
		//AnimationData animationData{};

		PlayerPushConstantData pushData{};
		TransformComponent transform{};
		EWECamera& camera;

		std::queue<TransformComponent> oldTransforms{};

		uint32_t animFrame = 0;
		CharmerSkeleton::Charmer_Animations animState = CharmerSkeleton::Anim_idle;

		glm::vec2 forwardDir{1.f / 250.f, 0.f};
		uint16_t barkFrames = 0;

		void movement(CharmerKeys polledKeys);
		void updateCarrotPet(CharmerKeys polledKeys);
		void updateZeroPet(CharmerKeys polledKeys);
		void updateForest(CharmerKeys polledKeys);
		void updateConnector(CharmerKeys polledKeys);

		void setZeroHistory();
	};
}

