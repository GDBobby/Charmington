#pragma once
#include "TileMap.h"
#include "../pipelines/PipelineEnum.h"

namespace EWE {
	class TileMapDevelopment {
	public:
		TileMapDevelopment(EWEDevice& device, uint32_t width, uint32_t height);
		~TileMapDevelopment();

		void refreshMap(EWEDevice& device, uint32_t width, uint32_t height);
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

		void saveMap(std::string saveLocation);

		bool refreshedMap = true;
	protected:
		//std::vector<glm::vec4> vertices{}; //i dont think i need to store vertices, but ok

		TileSet tileSet;

		std::map<uint32_t, uint64_t> tileIndexingMap{}; //tilePositionID, indices index

		std::unique_ptr<EWEModel> tileModel{ nullptr };

		uint64_t instanceCount = 0;
		uint32_t width, height;

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
		enum Borders : uint8_t {
			B_left = 1,
			B_top = 2,
			B_right = 4,
			B_bottom = 8,
		};

	};
}
