#include "TileSet.h"

#include <EWEngine/graphics/model/EWE_Basic_Model.h>
#include <EWEngine/graphics/EWE_Texture.h>

namespace EWE {
	TileSet::TileSet(EWEDevice& device, TileSet_Enum map_id) : setID{ map_id } {
		switch (map_id) {
			case TS_First: {
				tileSize = 32;
				width = 64;
				height = 19;
				tileScale = 0.5f;
				tileSetTexture = EWETexture::addSceneTexture(device, "tileSet.png");
				grassTiles.push_back(476);
				break;
			}
			default: {
				printf("trying to get variables for a map that doesn't exist \n");
				throw std::runtime_error("invalid map id");
				break;
			}
		}
		tileModel = Basic_Model::generate3DTileQuad(device, glm::vec2{ 1.f / width, 1.f / height });

	}

	void TileSet::setUVOffset(TileID tileID, glm::vec2& offset) {

		offset.x = static_cast<float>(tileID % width) / static_cast<float>(width);
		offset.y = std::floor(static_cast<float>(tileID) / static_cast<float>(width)) / static_cast<float>(height);
		printf("tile uv offset - %u : %.5f:%.5f - %u:%u \n", tileID, offset.x, offset.y, width, height);
	}

	TileFlag TileSet::getTileFlag(TileID tileID) {
		switch (tileID) {
		case 477: {
			return TileFlag_solid;
		}
		case 86: {
			return TileFlag_exit1;
		}
		default: {
			return TileFlag_none;
		}
		}
	}
}