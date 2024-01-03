#pragma once

#include <EWEngine/graphics/model/EWE_Model.h>

#include "TileSet.h"
#include <array>


namespace EWE {
	class TileMap {
	public:
		uint32_t width;
		uint32_t height;
		TileSet tileSet;
		TextureID textureID;
		std::vector<TileFlag> tileFlags;

		TileMap(EWEDevice& device, std::string fileLocation, TileSet::TileSet_Enum tileSetID);

		bool buildTileSquare(uint32_t tileID, TransformComponent& transform, glm::mat4& ret, glm::vec2& uvOffset);

		std::array<std::unique_ptr<EWEModel>, 5> grassField{nullptr, nullptr, nullptr, nullptr, nullptr};

		void renderGrass(float grassTime, uint8_t frameIndex);
		void renderTiles(uint8_t frameIndex);
	private:
		static TextureID grassTextureID;
		std::unique_ptr<EWEBuffer> tileVertexBuffer{ nullptr };
		std::unique_ptr<EWEBuffer> tileIndexBuffer{ nullptr };
		VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
	};
}
