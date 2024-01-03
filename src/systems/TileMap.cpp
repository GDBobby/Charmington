#include "TileMap.h"

#include <EWEngine/graphics/model/EWE_Vertex.h>
#include <EWEngine/graphics/EWE_Texture.h>
#include <EWEngine/systems/PipelineSystem.h>

#include "../pipelines/PipelineEnum.h"
#include "../pipelines/BackgroundPipe.h"

#include <fstream>
#include <filesystem>

const std::array<glm::vec4, 4> baseVertices = {
			glm::vec4(0.5f, 0.0f, 0.5f, 1.f),
			glm::vec4(-0.5f, 0.0f, 0.5f, 1.f),
			glm::vec4(-0.5f, 0.0f, -0.5f, 1.f),
			glm::vec4(0.5f, 0.0f, -0.5f, 1.f),
};
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

namespace EWE {
	TextureID TileMap::grassTextureID{TEXTURE_UNBINDED};

	bool TileMap::buildTileSquare(uint32_t tileID, TransformComponent& transform, glm::mat4& ret, glm::vec2& UVOffset) {
		transform.scale.x = tileSet.tileScale;
		transform.scale.y = tileSet.tileScale;
		transform.rotation.y = 0.f;
		if (tileID & FLIPPED_HORIZONTALLY_FLAG) {
			transform.scale.x *= -1.f;
		}
		if ((tileID & FLIPPED_DIAGONALLY_FLAG)) {
			transform.rotation.y = glm::pi<float>();
		}
		if (tileID & FLIPPED_VERTICALLY_FLAG) {
			transform.scale.z *= -1.f;
		}

		ret = transform.mat4();
		
		tileID &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
		tileFlags.push_back(tileSet.getTileFlag(tileID));

		tileSet.setUVOffset(tileID, UVOffset);

		return false;
	}

	TileMap::TileMap(EWEDevice& device, std::string fileLocation, TileSet::TileSet_Enum tileSetID) : tileSet{device, tileSetID } {
		std::string fileReal = "models/";
		fileReal += fileLocation;
		std::ifstream inStream{ fileReal };
		if (!inStream.is_open()) {
			if (!std::filesystem::exists(fileLocation)) {
				printf("loaded map doesn't exist : %s \n", fileLocation.c_str());
				throw std::runtime_error("inexistant file \n");
			}
			inStream.open(fileLocation);
			if (!inStream.is_open()) {
				printf("failed to load map twice : %s \n", fileLocation.c_str());
				throw std::runtime_error("failed to loadm ap twice");
			}
		}
		inStream >> width;
		inStream >> height;
		if ((width == 0) || (height == 0)) {
			printf("invalid map size \n");
			throw std::runtime_error("invalid map size");
		}
		if (grassTextureID == TEXTURE_UNBINDED) {
			grassTextureID = EWETexture::addSceneTexture(device, "noise.ppm", EWETexture::tType_simpleVert);
			printf("redwind value - %d \n", grassTextureID);
		}


		uint32_t tileCount = width * height;
		tileFlags.reserve(tileCount);
		TransformComponent transform;
		transform.scale = glm::vec3{tileSet.tileScale};
		std::vector<uint32_t> indices{};
		indices.reserve(tileCount * 6);
		std::vector<glm::vec4> outVertices{};
		outVertices.reserve((width + 1) * (height + 1));
		glm::vec3 baseTranslation{ -static_cast<float>(width) * tileSet.tileScale / 2.f, 0.f, -static_cast<float>(height) * tileSet.tileScale / 2.f };
		printf("base translation - %.5f:%.5f:%.5f\n", baseTranslation.x, baseTranslation.y, baseTranslation.z);
		transform.translation = baseTranslation;
		glm::vec2 offsetPerTile{ 0.5f };
		glm::vec2 uvScaling{ 1.f / tileSet.width, 1.f / tileSet.height };

		std::vector<glm::vec2> tileUVOffset(width * height);

		std::array<std::vector<GrassInstance>, 5> grassMats{};
		for (int i = 0; i < 5; i++) {
			grassMats[i].reserve(width * height);
		}
		glm::vec3 grassRotations[4] = {
			//		cos	sin
			/*0*/	{0.f, 0.f, 0.f},
			/*90*/	{0.f, glm::half_pi<float>(), 0.f},
			/*180*/	{0.f, glm::pi<float>(), 0.f},
			/*270*/	{0.f, glm::quarter_pi<float>() * 3.f, 0.f}
		};
		int grassRotationCounter[5] = { 0,0,0,0,0 };
		int grassCounter = 0;
		TransformComponent grassTransform{};


		uint32_t tileBuffer;
		inStream >> tileBuffer;
		glm::mat4 mat4{};

		//square 0,0
		if (buildTileSquare(tileBuffer, transform, mat4, tileUVOffset[0])) {
			grassTransform.translation = transform.translation;
			grassTransform.rotation = grassRotations[grassRotationCounter[grassCounter]];
			grassRotationCounter[grassCounter] = (grassRotationCounter[grassCounter] + 1) % 4;
			grassCounter = (grassCounter + 1) % 5;
			grassMats[0].push_back(grassTransform.mat4());
		}

		indices.push_back(0);

		//glm::vec4 buffer = mat4 * baseVertices[0];
		//printf("first method : (%.5f):(%.5f):(%.5f):(%.5f)\n", buffer.x, buffer.y, buffer.z, buffer.w);
		//buffer = baseVertices[0] * mat4;
		//printf("second method : (%.5f):(%.5f):(%.5f):(%.5f)\n", buffer.x, buffer.y, buffer.z, buffer.w);

		outVertices.push_back(mat4 * baseVertices[0]);

		indices.push_back(1);
		glm::vec4 buffer = mat4 * baseVertices[1];
		printf("second vertex : (%.5f):(%.5f):(%.5f):(%.5f)\n", buffer.x, buffer.y, buffer.z, buffer.w);
		outVertices.push_back(mat4 * baseVertices[1]);

		indices.push_back(2);
		outVertices.push_back(mat4 * baseVertices[2]);
		indices.push_back(2);

		indices.push_back(3);
		outVertices.push_back(mat4 * baseVertices[3]);

		indices.push_back(0);

		//row 1 - 0,1 - 0,2 - 0,3...
		for (int x = 1; x < width; x++) {
			inStream >> tileBuffer;
			transform.translation = baseTranslation;
			transform.translation.x += offsetPerTile.x * static_cast<float>(x);
			if (buildTileSquare(tileBuffer, transform, mat4, tileUVOffset[x])) {
				grassTransform.translation = transform.translation;
				grassTransform.rotation = grassRotations[grassRotationCounter[grassCounter]];
				grassRotationCounter[grassCounter] = (grassRotationCounter[grassCounter] + 1) % 4;
				grassCounter = (grassCounter + 1) % 5;
				grassMats[x % 5].push_back(grassTransform.mat4());
			}

			indices.push_back(outVertices.size());
			outVertices.push_back(mat4 * baseVertices[0]);

			indices.push_back(indices[indices.size() - 7]);
			indices.push_back(indices[indices.size() - 4]);
			indices.push_back(indices.back());


			indices.push_back(outVertices.size());
			outVertices.push_back(mat4 * baseVertices[3]);
			indices.push_back(outVertices.size() - 2);

		}

		for (int y = 1; y < height; y++) {
			inStream >> tileBuffer;
			transform.translation = baseTranslation;
			transform.translation.z += offsetPerTile.y * static_cast<float>(y);
			if (buildTileSquare(tileBuffer, transform, mat4, tileUVOffset[y * width])) {
				grassTransform.translation = transform.translation;
				grassTransform.rotation = grassRotations[grassRotationCounter[grassCounter]];
				grassRotationCounter[grassCounter] = (grassRotationCounter[grassCounter] + 1) % 4;
				grassCounter = (grassCounter + 1) % 5;
				grassMats[y * 2].push_back(grassTransform.mat4());
			}
			indices.push_back(outVertices.size());

			outVertices.push_back(mat4 * baseVertices[0]);


			indices.push_back(outVertices.size());

			outVertices.push_back(mat4 * baseVertices[1]);

			indices.push_back(indices[indices.size() - (width * 6 + 1)]);
			indices.push_back(indices.back());

			indices.push_back(indices[indices.size() - (width * 6 + 4)]);

			indices.push_back(indices[indices.size() - 5]);


			//square 1 1

			for (int x = 1; x < width; x++) {
				inStream >> tileBuffer;
				transform.translation.x = baseTranslation.x + offsetPerTile.x * static_cast<float>(x);
				if (buildTileSquare(tileBuffer, transform, mat4, tileUVOffset[y * width + x])) {
					grassTransform.translation = transform.translation;
					grassTransform.rotation = grassRotations[grassRotationCounter[grassCounter]];
					grassRotationCounter[grassCounter] = (grassRotationCounter[grassCounter] + 1) % 4;
					grassCounter = (grassCounter + 1) % 5;
					grassMats[(y * 2 + x) % grassMats.size()].push_back(grassTransform.mat4());
				}

				indices.push_back(outVertices.size());
				outVertices.push_back(mat4 * baseVertices[0]);

				indices.push_back(indices[indices.size() - 7]);

				indices.push_back(indices[indices.size() - (width * 6 + 1)]);
				indices.push_back(indices.back());

				indices.push_back(indices[indices.size() - (width * 6 + 4)]);

				indices.push_back(outVertices.size() - 1);

			}
		}

		//uint32_t totalPatchCount = 0;
		for (uint8_t i = 0; i < grassField.size(); i++) {
			//totalPatchCount += static_cast<uint32_t>(instanceTranslation[i].size());
			//printf("size of instancing buffer[%d] : %d \n", i, instanceTranslation[i].size() * sizeof(EWEModel::GrassInstance));
			if (grassMats[i].size() != 0) {
				grassField[i] = EWEModel::createGrassModelFromFile(device, grassNames[i]);

				grassField[i]->AddInstancing(static_cast<uint32_t>(grassMats[i].size()), sizeof(GrassInstance), grassMats[i].data());
			}
		}

		tileVertexBuffer = std::make_unique<EWEBuffer>(device, sizeof(glm::vec4) * outVertices.size(), 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		tileVertexBuffer->map();

		printf("storage buffer alignment : %lu \n", device.getProperties().limits.minStorageBufferOffsetAlignment);
		printf("tile vertex 1 : %.2f:%.2f:%.2f:%.2f \n", outVertices[0].x, outVertices[1].y, outVertices[2].z, outVertices[3].w);
		//for (int i = 0; i < outVertices.size(); i++) {
		//	tileVertexBuffer->writeToBufferAligned(&outVertices[i], sizeof(outVertices[0]), i);
		//}
		tileVertexBuffer->writeToBuffer(outVertices.data(), outVertices.size() * sizeof(outVertices[0]));
		tileVertexBuffer->flush();
		tileSet.tileModel->AddInstancing(tileUVOffset.size(), sizeof(glm::vec2), tileUVOffset.data());

		tileIndexBuffer = std::make_unique<EWEBuffer>(device, sizeof(uint32_t) * indices.size(), 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		tileIndexBuffer->map();
		//for (int i = 0; i < indices.size(); i++) {
		//	tileIndexBuffer->writeToBufferAligned(&indices[i], sizeof(uint32_t), i);
		//}

		tileIndexBuffer->writeToBuffer(indices.data(), indices.size() * sizeof(indices[0]));
		tileVertexBuffer->flush();
		//bufferMap[Buff_loading][frameIndex]->writeToBuffer(leafSystem->getLeafTransformBuffer(), LEAF_COUNT * sizeof(glm::mat4));
		//just need to create the storage buffer for the index and vertices now

		if (!
			EWEDescriptorWriter(((BackgroundPipe*)PipelineSystem::at(Pipe_background))->getVertexIndexBufferLayout(), DescriptorPool_Global)
			.writeBuffer(0, tileVertexBuffer->descriptorInfo())
			.writeBuffer(1, tileIndexBuffer->descriptorInfo())
			.build(descriptorSet)
			) {
			printf("tile desc failure \n");
			throw std::runtime_error("failed to build tile map descriptor set");
		}
	}

	void TileMap::renderGrass(float grassTime, uint8_t frameIndex) {
		auto pipe = PipelineSystem::at(Pipe_grass2);
		pipe->bindPipeline();

		pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameIndex));
		pipe->bindDescriptor(1, EWETexture::getDescriptorSets(grassTextureID, frameIndex));
		UVScrollingPushData push{ glm::vec2{glm::mod(grassTime / 6.f, 1.f), glm::mod(grassTime / 9.f, 1.f)} };
		//printf("uv scroll : %.5f:%.5f \n", push.uvScroll.x, push.uvScroll.y);
		pipe->push(&push);

		for (int i = 0; i < grassField.size(); i++) {
			if (grassField[i] != nullptr) {
				pipe->drawInstanced(grassField[i].get());
			}
		}
	}
	void TileMap::renderTiles(uint8_t frameIndex) {
		auto pipe = PipelineSystem::at(Pipe_background);

		pipe->bindPipeline();
		//pipe->bindModel(tileSet.tileModel.get());

		pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameIndex));
		pipe->bindDescriptor(1, &descriptorSet);
		pipe->bindDescriptor(2, EWETexture::getDescriptorSets(tileSet.tileSetTexture, frameIndex));

		//ModelPushData push;
		//push.modelMatrix = floorTransform.mat4();
		//push.normalMatrix = floorTransform.normalMatrix();

		UVScrollingPushData push{};
		push.uvScroll.x = 1.f / static_cast<float>(tileSet.width);
		push.uvScroll.y = 1.f / static_cast<float>(tileSet.height);
		pipe->push(&push);

		pipe->drawInstanced(tileSet.tileModel.get());
	}
}