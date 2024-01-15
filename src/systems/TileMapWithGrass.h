#pragma once
#include "TileMap.h"

#include <EWEngine/Graphics/Frame_Info.h>

namespace EWE {
	class TileMapWithGrass : public TileMap {
	public:
		TileMapWithGrass(EWEDevice& device, std::string fileLocation, TileSet::TileSet_Enum tileSetID);

		std::array<std::unique_ptr<EWEModel>, 5> grassField{ nullptr, nullptr, nullptr, nullptr, nullptr };

		void buildTileMap(EWEDevice& device, std::string const& fileLocation,
			std::vector<glm::vec4>& outVertices, std::vector<uint32_t>& indices) override;
	protected:
		static TextureID grassTextureID;
		void buildTileSquare(uint32_t& tileID, TransformComponent& transform, glm::mat4& ret, glm::vec2& uvOffset);

		void renderGrass(FrameInfo& frameInfo);
		float grassTime = 0.f;

		struct GrassCreation {
			TransformComponent transform{};
			int rotationCounter[5] = { 0,0,0,0,0 };
			int counter = 0;
			std::array<std::vector<GrassInstance>, 5> mats;

		};
		GrassCreation* grassCreation{ nullptr };
	};
}

