#pragma once

#include <EWEngine/GameObject2D.h>
#include <EWEngine/graphics/TextOverlay.h>
#include <EWEngine/graphics/model/EWE_Model.h>
#include <EWEngine/GUI/Overlay.h>


namespace EWE {
	class CharmerOverlay : public OverlayBase {
	public:
		enum OverlayObjects_Enum {
			logs,
			carrot,
			zero,
			sheet,


			objects_size,
		};

		enum BattleString_Enum {
			BS_logCount,
			BS_carrotKB,

		};
		CharmerOverlay(EWEDevice& device, float screenWidth, float screenHeight, std::shared_ptr<TextOverlay> textOverlay);

		void reset();
		//void toggleDraw();
		void SetDrawState(bool drawState);

		void drawText() override;

		void drawObjects(std::pair<VkCommandBuffer, uint8_t> cmdIndexPair);

		void updateFromSettings();


		uint16_t beginRoundFrames = 0;
	private:
		glm::vec4 getHPPushData(uint8_t playerIndex);

		uint8_t* logCount;
		char carrotKeybind;
		bool tamedCarrot = false;
		bool tamedZero = false;
		bool tamedSheet = false;

		TextureID hpContainer;
	};
}