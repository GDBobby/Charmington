#pragma once
#include <EWEngine/imgui/imgui.h>
#include <EWEngine/Data/TransformInclude.h>
#include <EWEngine/Data/EngineDataTypes.h>

#include <queue>

namespace EWE {
	class LevelCreationIMGUI {
	public:
		LevelCreationIMGUI(std::queue<uint16_t>& clickReturns, float screenWidth, float screenHeight);

		void render(uint8_t frameIndex);

        void ShowMainMenuBar();
		void ShowMenuFile();
		void ShowGridControl();
		void ShowTileSet();

		//void addTextures();

		bool showGrid = false;

		glm::vec2* gridScale = nullptr;
		glm::vec2* gridTrans = nullptr;
		float scaleLow = 0.f;
		float scaleHigh = 2.f;
		float transLow = -100.f;
		float transHigh = 100.f;
		float tileSetScale = 128.f;
		float tileSetRatio = 1.f;

		//float tileSetScaleY = 1.f;
		float tileSetScaleLow = 128.f;
		float tileSetScaleHigh = 4096.f;

		float screenWidth;
		float screenHeight;

		int selectedTile = 0;
		ImVec2 selectedTileUVBR{0.f,0.f};
		ImVec2 selectedTileUVTL{1.f / 64.f, 1.f / 19.f};

		ImVec2 toolUV;
		ImVec2 toolUVBR;

		ImVec2 tileUVScaling = { 1.f / 64.f, 1.f / 19.f };

		bool hoveringTileSet = false;

		std::queue<uint16_t>& clickReturns;
		int toolSelectedTile = 0;

		TextureID tileSetID;

		bool gridControlOpen = true;

		void mouseCallback(int button, int action);
	};
}

