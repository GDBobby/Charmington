#pragma once

#include <EWEngine/Data/EngineDataTypes.h>
#include <EWEngine/Data/TransformInclude.h>

#include <stdexcept>
#include <string>
#include <vector>

typedef uint64_t TileID;
enum class TileFlag {
	none,
	solid,
	slow,
	exit,
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
		//std::string fileLocation;

		TileSet(TileSet_Enum map_id);
		void interpretTileID(TileID& tileID);//, float& rotation);

		void setUVOffset(TileID tileID, glm::vec2& offset);

		TileFlag getTileFlag(TileID tileID);
	};

	struct TileMap {
		uint32_t width;
		uint32_t height;
		TileSet tileSet;
		TextureID textureID;

		TileMap(std::string fileLocation, TileSet::TileSet_Enum tileSetID) : tileSet{tileSetID} {

		}

	};
}