#pragma once
#include <EWEngine/imgui/imgui.h>
#include <EWEngine/Data/TransformInclude.h>
#include <EWEngine/Data/EngineDataTypes.h>

#include <EWEngine/graphics/EWE_Device.hpp>

#include <queue>

namespace EWE {
	class LevelCreationIMGUI {
	public:
		enum Tool_Enum : uint16_t {
			Tool_pencil,
			Tool_eraser,
		};
		Tool_Enum selectedTool = Tool_pencil;

		LevelCreationIMGUI(std::queue<uint16_t>& clickReturns, float screenWidth, float screenHeight);

		void render();
		void loadTextures(EWEDevice& device);

        void ShowMainMenuBar();
		void ShowMenuFile();
		void ShowGridControl();
		void ShowTileSet();
		void ShowNewPrompt();
		void ShowToolControls();

		//void addTextures();

		uint32_t tileSize = 32;

		bool showGrid = false;

		glm::vec2* gridZoom = nullptr;
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

		int64_t selectedTile = 0;
		ImVec2 selectedTileUVBR{0.f,0.f};
		ImVec2 selectedTileUVTL{1.f / 64.f, 1.f / 19.f};

		ImVec2 toolUV;
		ImVec2 toolUVBR;

		ImVec2 tileUVScaling = { 1.f / 64.f, 1.f / 19.f };

		bool hoveringTileSet = false;

		std::queue<uint16_t>& clickReturns;
		int toolSelectedTile = 0;

		TextureID tileSetID;
		TextureID pencilTextureID;
		ImVec4 pencilColor{0.f,0.f,0.f,1.f};
		ImVec4 eraserColor{ 0.f,0.f,0.f,1.f };
		ImVec4 selectedColor{ 1.f,1.f,1.f,1.f };
		ImVec4 idleColor{ 0.f,0.f,0.f,1.f };

		TextureID eraserTextureID;

		bool gridControlOpen = true;

		void mouseCallback(int button, int action);
		void scrollCallback(double yOffset);

		bool levelNew = false;

		int levelWidth = 8;
		int levelHeight = 8;

		void (*createButtonPtr)(uint32_t, uint32_t);

	};
}

