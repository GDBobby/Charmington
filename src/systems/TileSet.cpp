#include "TileSet.h"

#include <EWEngine/graphics/model/EWE_Basic_Model.h>
#include <EWEngine/graphics/EWE_Texture.h>


const uint32_t FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const uint32_t FLIPPED_VERTICALLY_FLAG = 0x40000000;
const uint32_t FLIPPED_DIAGONALLY_FLAG = 0x20000000;

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

	std::array<glm::vec2, 4> TileSet::getUVOffset(TileID tileID) {
		//clockwise, from top left

		std::array<glm::vec2, 4> offsets;

		glm::vec2 baseOffset{ static_cast<float>(tileID % width) / static_cast<float>(width), std::floor(static_cast<float>(tileID) / static_cast<float>(width)) / static_cast<float>(height) };

		offsets[0] = baseOffset;

		offsets[2] = baseOffset;
		offsets[2].x += 1.f / static_cast<float>(width);
		offsets[2].y += 1.f / static_cast<float>(height);

		offsets[1].x = offsets[2].x;
		offsets[1].y = baseOffset.y;

		offsets[3].x = baseOffset.x;
		offsets[3].y = offsets[2].y;


		if (tileID & FLIPPED_HORIZONTALLY_FLAG) {
			std::swap(offsets[0], offsets[1]);
			std::swap(offsets[3], offsets[2]);
		}
		if ((tileID & FLIPPED_DIAGONALLY_FLAG)) {
			//1->4 or 0 -> 5? doesnt matter i guess
			std::swap(offsets[1], offsets[3]);
		}
		if (tileID & FLIPPED_VERTICALLY_FLAG) {
			std::swap(offsets[0], offsets[3]);
			std::swap(offsets[1], offsets[2]);
		}

		printf("uv offsets : (%.5f:%.5f)(%.5f:%.5f)(%.5f:%.5f)(%.5f:%.5f) \n", offsets[0].x, offsets[0].y, offsets[1].x, offsets[1].y, offsets[2].x, offsets[2].y, offsets[3].x, offsets[3].y);

		return offsets;
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