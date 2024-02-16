#include "TileMapWithGrass.h"

#include <EWEngine/Graphics/Texture/Texture_Manager.h>
#include <EWEngine/Systems/PipelineSystem.h>
#include "../pipelines/PipelineEnum.h"



const uint32_t FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const uint32_t FLIPPED_VERTICALLY_FLAG = 0x40000000;
const uint32_t FLIPPED_DIAGONALLY_FLAG = 0x20000000;

const std::string grassNames[5] = {
	"grassPatch0.obj",
	"grassPatch1.obj",
	"grassPatch2.obj",
	"grassPatch3.obj",
	"grassPatch4.obj",
};

const glm::vec3 grassRotations[4] = {
	//		cos	sin
	/*0*/	{0.f, 0.f, 0.f},
	/*90*/	{0.f, glm::half_pi<float>(), 0.f},
	/*180*/	{0.f, glm::pi<float>(), 0.f},
	/*270*/	{0.f, glm::quarter_pi<float>() * 3.f, 0.f}
};

namespace EWE {

	void TileMapWithGrass::buildTileSquare(uint32_t& tileID, TransformComponent& transform, glm::mat4& ret, glm::vec2& uvOffset) {
		//TileMap::buildTileSquare(tileID, transform, ret, uvOffset);
		for (auto grassTile : tileSet.grassTiles) {
			if (tileID == grassTile) {
				grassCreation->transform.translation = transform.translation;

				grassCreation->transform.rotation = grassRotations[grassCreation->rotationCounter[grassCreation->counter]];
				grassCreation->rotationCounter[grassCreation->counter] = (grassCreation->rotationCounter[grassCreation->counter] + 1) % 4;
				grassCreation->counter = (grassCreation->counter + 1) % 5;
				grassCreation->mats[0].push_back(grassCreation->transform.mat4());
			}
		}

	}

	TileMapWithGrass::TileMapWithGrass(EWEDevice& device, std::string fileLocation, TileSet::TileSet_Enum tileSetID) : TileMap{device, fileLocation, tileSetID} {
		grassTextureID = Texture_Builder::createSimpleTexture( "noise.ppm", false, false, VK_SHADER_STAGE_VERTEX_BIT);
		
	}

	void TileMapWithGrass::buildTileMap(EWEDevice& device, std::string const& fileLocation,
		std::vector<glm::vec4>& outVertices, std::vector<uint32_t>& indices) {
		grassCreation = new GrassCreation{};
		for (int i = 0; i < 5; i++) {
			grassCreation->mats[i].reserve(width * height);
		}


		TileMap::buildTileMap(device, fileLocation, outVertices, indices);

		for (uint8_t i = 0; i < grassField.size(); i++) {
			//totalPatchCount += static_cast<uint32_t>(instanceTranslation[i].size());
			//printf("size of instancing buffer[%d] : %d \n", i, instanceTranslation[i].size() * sizeof(EWEModel::GrassInstance));
			if (grassCreation->mats[i].size() != 0) {
				grassField[i] = EWEModel::createGrassModelFromFile(device, grassNames[i]);

				grassField[i]->AddInstancing(static_cast<uint32_t>(grassCreation->mats[i].size()), sizeof(GrassInstance), grassCreation->mats[i].data());
			}
		}
		delete grassCreation;
	}

	void TileMapWithGrass::renderGrass(FrameInfo& frameInfo, float dt) {
		grassTime += dt;	
		auto pipe = PipelineSystem::at(Pipe_grass2);
		pipe->bindPipeline();

		pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameInfo.index));
		pipe->bindDescriptor(1, &grassTextureID);
		UVScrollingPushData push{ glm::vec2{glm::mod(grassTime / 6.f, 1.f), glm::mod(grassTime / 9.f, 1.f)} };
		//printf("uv scroll : %.5f:%.5f \n", push.uvScroll.x, push.uvScroll.y);
		pipe->push(&push);

		for (int i = 0; i < grassField.size(); i++) {
			if (grassField[i] != nullptr) {
				pipe->drawInstanced(grassField[i].get());
			}
		}
	}
}