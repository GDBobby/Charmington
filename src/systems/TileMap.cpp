#include "TileMap.h"

namespace EWE {
	TileSet::TileSet(TileSet_Enum map_id) : setID{ map_id } {
		switch (map_id) {
		case TS_First: {
			tileSize = 32;
			width = 64;
			height = 19;
			//fileLocation = "textures/tileSet.png";
			break;
		}
		default: {
			printf("trying to get variables for a map that doesn't exist \n");
			throw std::runtime_error("invalid map id");
			break;
		}
		}
	}

	void TileSet::interpretTileID(TileID& tileID) {
		switch (setID) {
		case TS_First: {
			switch (tileID) {
			case 2684355063: {
				tileID = 503;
				//rotation = -glm::half_pi<float>();
				break;
			}
			case 3221225975: {
				tileID = 503;
				//rotation = glm::pi<float>();
				break;
			}
			case 1610613239: {
				tileID = 503;
				//rotation = glm::half_pi<float>();
				break;
			}
			case 3758096887: {
				tileID = 503;
				//rotation = glm::half_pi<float>();
				break;
			}
			case 2147484152: {
				tileID = 495;
				break;
			}
			case 1610613228: {
				tileID = 495;
				break;
			}
			case 1610613206: {
				tileID = 495;
				break;
			}
			case 3221225942: {
				tileID = 495;
				break;
			}
			case 536871382: {
				tileID = 495;
				break;
			}
			case 2684355030: {
				tileID = 495;
				break;
			}
			case 3221225964: {
				tileID = 495;
				break;
			}
			case 2684355052: {
				tileID = 495;
				break;
			}
			case 493: {
				tileID = 499;
				break;
			}
			case 499: {
				tileID = 493;
				break;
			}
			case 491: {
				tileID = 497;
				break;
			}
			case 497: {
				tileID = 491;
				break;
			}

			default: {
				break;
			}
			}
		}
		}
		
		if (tileID > (width * height)) {
			printf("invalid tile, probably a rotated tile. need to add support : %lu \n", tileID);
			throw std::runtime_error("invalid tile");
		}
	}
	void TileSet::setUVOffset(TileID tileID, glm::vec2& offset) {

		offset.x = static_cast<float>(tileID % width) / static_cast<float>(width);
		offset.y = std::floor(static_cast<float>(tileID) / static_cast<float>(width)) / static_cast<float>(height);
		//printf("tile uv offset - %u : %.5f:%.5f - %u:%u \n", tileID, offset.x, offset.y, width, height);
	}

	TileFlag TileSet::getTileFlag(TileID tileID) {
		switch (tileID) {
		case 477: {
			return TileFlag_solid;
		}
		case 86:{
			return TileFlag_exit1;
		}
		default: {
			return TileFlag_none;
		}
		}
	}
}