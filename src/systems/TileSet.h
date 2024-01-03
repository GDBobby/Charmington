#pragma once

#include <EWEngine/Data/EngineDataTypes.h>
#include <EWEngine/Data/TransformInclude.h>
#include <EWEngine/graphics/model/EWE_Model.h>

#include <stdexcept>
#include <string>
#include <vector>

typedef uint64_t TileID;
enum TileFlag : uint16_t {
	TileFlag_none,
	TileFlag_solid,
	TileFlag_slow,
	TileFlag_exit1,
	TileFlag_exit2,
	TileFlag_exit3,
	TileFlag_exit4,
	TileFlag_exit5,
	TileFlag_exit6,
};

namespace EWE {
	struct TileSet {
		enum TileSet_Enum {
			TS_First,
			//TS_Start,
		};

		TileSet_Enum setID;
		uint32_t tileSize;
		uint32_t width;
		uint32_t height;
		float tileScale;
		//std::string fileLocation;

		TileSet(EWEDevice& device, TileSet_Enum map_id);

		void setUVOffset(TileID tileID, glm::vec2& offset);

		TileFlag getTileFlag(TileID tileID);

		std::vector<uint32_t> grassTiles{};
		std::vector<uint32_t> solidTiles{};
		std::vector<uint32_t> exitTiles{};

		std::unique_ptr<EWEModel> tileModel{ nullptr };
		TextureID tileSetTexture = TEXTURE_UNBINDED;
	};
}