#pragma once
#include "TileMap.h"
#include "../pipelines/PipelineEnum.h"
#include "TileContainer.h"

#include <queue>

namespace EWE {
	class TileMapDevelopment {
	public:
		TileMapDevelopment(EWEDevice& device, uint16_t width, uint16_t height);
		~TileMapDevelopment();

		void refreshMap(uint16_t width, uint16_t height);
		void renderTiles(VkCommandBuffer cmdBuf, uint8_t frameIndex);

		std::array<float, 6> const& getScreenCoordinates(float screenWidth, float screenHeight) {
			if (refreshedMap) {
				refreshedMap = false;
				return updateScreenCoordinates(screenWidth, screenHeight);
			}
			printf("screenCoordinates : \n\t");
			for (int i = 0; i < screenCoordinates.size(); i++) {
				printf("(%.2f)", screenCoordinates[i]);
			}
			printf("\n");
			return screenCoordinates;
		}

		int64_t getClickedTile(float clickX, float clickY, float screenWidth, float screenHeight);

		void changeTile(uint32_t tilePosition, TileID tileID);
		void removeTile(uint32_t tilePosition);

		void fitToScreen();
		PushTileConstantData pushTile{};

		bool saveMap(std::string saveLocation);
		bool loadMap(std::string loadLocation);

		void colorSelection(uint32_t selectPosition);
		void selectNeighbor(uint32_t selectPosition, int64_t selID, std::queue<int64_t>& selection);
		void bucketFill(uint32_t clickedTilePosition, TileID selectedTile) {
			tileContainer->bucketFill(clickedTilePosition, selectedTile);
		}

		bool refreshedMap = true;
	protected:
		//std::vector<glm::vec4> vertices{}; //i dont think i need to store vertices, but ok

		TileSet tileSet;

		struct TileInfo {
			uint64_t memoryLocation;
			TileID tileID;
			TileInfo(uint64_t memLoc, TileID tileID) : memoryLocation(memLoc), tileID(tileID) {}
		};

		TileContainer* tileContainer;
		//std::map<uint32_t, TileInfo> tileIndexingMap{}; //tilePositionID, indices/uv memLocation

		//uint64_t instanceCount = 0;
		uint16_t width, height;

		std::array<float, 6> screenCoordinates{};

		std::unique_ptr<EWEBuffer> tileVertexBuffer{ nullptr };
		std::unique_ptr<EWEBuffer> tileIndexBuffer{ nullptr };
		std::unique_ptr<EWEBuffer> tileUVBuffer{ nullptr };
		VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };


		void init(EWEDevice& device);
		void constructVertices(EWEDevice& device, float tileScale);
		void constructUVsAndIndices(EWEDevice& device);
		void deconstructMap();
		void constructDescriptor();

		std::array<float, 6> const& updateScreenCoordinates(float screenWidth, float screenHeight);

		std::unique_ptr<EWEBuffer> modelIndexBuffer;

		uint32_t borders = 0;


		EWEDevice& device;

	};
}

