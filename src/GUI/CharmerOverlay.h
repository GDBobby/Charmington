#pragma once

#include <EWEngine/GameObject2D.h>
#include <EWEngine/Graphics/TextOverlay.h>
#include <EWEngine/Graphics/Model/Model.h>
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

		enum CharmerString_Enum : uint16_t {
			BS_logCount,
			BS_carrotKB,
			BS_zeroKB,

			BS_size,
		};
		CharmerOverlay(EWEDevice& device, float screenWidth, float screenHeight, uint8_t& logCount);

		//void toggleDraw();
		void SetDrawState(bool drawState);

		void drawText() override;

		void drawObjects(std::pair<VkCommandBuffer, uint8_t> cmdIndexPair);

		void updateFromSettings();


		uint16_t beginRoundFrames = 0;
	private:

		std::vector<TextureID> objectTextures{};
		std::vector<Transform2dComponent> overlayBackground{};
		TextureID overlayBackID;

		uint8_t& logCount;
		char carrotKeybind;
		bool tamedCarrot = false;
		bool tamedZero = false;
		bool tamedSheet = false;

		TextureID hpContainer;
	};
}