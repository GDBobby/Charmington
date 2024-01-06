#pragma once
#include "TileMap.h"
#include "../pipelines/PipelineEnum.h"

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

		bool refreshedMap = true;
	protected:
		//std::vector<glm::vec4> vertices{}; //i dont think i need to store vertices, but ok

		TileSet tileSet;

		struct TileInfo {
			uint64_t memoryLocation;
			TileID tileID;
			TileInfo(uint64_t memLoc, TileID tileID) : memoryLocation(memLoc), tileID(tileID) {}
		};
		struct TileNode {
			TileID tileID;
			uint32_t nextPosition;
		};
		struct TileContainer {
		protected:
			uint32_t beginPosition;
			uint32_t count;
			uint32_t size;
			uint32_t finalPosition;

			TileNode* tileMemory;

			//i wanna know when i have contiguous
			//thinking second buffer, just for sparse tracking, question is
			//how do i track regions of contiguous memory


		public:
			uint32_t begin() {
				return beginPosition;
			}
			uint32_t count() {
				return count;
			}
			uint32_t size() {

			}
			void writeToBuffer(void* buffer) {
				char* buf = reinterpret_cast<char*>(buffer);
				uint64_t memLoc = 0;
				TileNode* tileNode = tileMemory + beginPosition;
				uint64_t tileOffset = offsetof(TileNode, tileID);

				uint32_t curPos = beginPosition;
				uint32_t dataSize;
				while (curPos != finalPosition) {
					memcpy(buf + memLoc, tileNode + tileOffset, 4);
					memLoc++;
				}
			}
		};

		std::map<uint32_t, TileInfo> tileIndexingMap{}; //tilePositionID, indices/uv memLocation

		std::unique_ptr<EWEModel> tileModel{ nullptr };

		uint64_t instanceCount = 0;
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
		enum Borders : uint8_t {
			B_left = 1,
			B_top = 2,
			B_right = 4,
			B_bottom = 8,
		};

		EWEDevice& device;

	};
}

