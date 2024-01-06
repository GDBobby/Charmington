#include "Level.h"

namespace EWE {
	

	void Terrain_Object::render(FrameInfo& frameInfo) {
		TextureID currentlyBinded = TEXTURE_UNBINDED;
		SimplePushConstantData push;

		model->bind(frameInfo.cmdIndexPair.first);
		for (auto& objectType : objectTypes) {
			/*
			if (objectType.first != currentlyBinded) {
				vkCmdBindDescriptorSets(frameInfo.cmdIndexPair.first,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					pipeLayout,
					1, 1,
					EWETexture::getDescriptorSets(objectType.first, frameInfo.cmdIndexPair.second),
					0, nullptr
				);
				currentlyBinded = objectType.first;
			}

			for (auto& object : objectType.second) {
				if (object.active) {
					push.modelMatrix = object.transform.mat4();
					push.normalMatrix = object.transform.normalMatrix();
					vkCmdPushConstants(frameInfo.cmdIndexPair.first, pipeLayout, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, sizeof(SimplePushConstantData), &push);
					model->draw(frameInfo.cmdIndexPair.first);
				}
			}
			*/
		}


	}

	void Level::render(FrameInfo& frameInfo) {

		{
			tileMap->renderTiles(frameInfo.cmdIndexPair.first, frameInfo.cmdIndexPair.second);

		}

		return;
	}
	/*
	TileFlag Level::tileFlagAt(float x, float y) {
#ifdef _DEBUG
		TileID tileID = tileAt(x, y);
		//printf("tile at %.2f:%.2f - %d\n", x, y, tileID);
		TileFlag tileFlag = tileSet.getTileFlag(tileID);
		if (tileFlag != TileFlag::none) {
			printf("tileFlag : %d \n", tileFlag);
		}
		return tileFlag;
#else
		return tileSet.getTileFlag(tileAt(x, y));
#endif
	}
	*/
	TileFlag Level::tileAt(float x, float y) {

		//printf("x ? %d \n", static_cast<int>(std::floor(x * 2.f)) + tileMap->width / 2);
		//printf("y ? %d \n", static_cast<int>(std::floor(y * 2.f)) * tileMap->height + (tileMap->width * tileMap->height / 2));
		//printf("tileMap->width ? %d \n", tileMap->width);
		//printf("tileMap->height ? %d \n", tileMap->height);

		return tileMap->tileFlags.at(static_cast<int>(std::floor(x * 2.f)) + tileMap->width / 2 + static_cast<int>(std::floor(y * 2.f)) * tileMap->width + (tileMap->width * tileMap->height / 2));
	}
	/*
	void Level::loadGrass(EWEDevice& device) {
		//LARGE_INTEGER queryStart;
		//QueryPerformanceCounter(&queryStart);

		grassTextureID = EWETexture::addSceneTexture(device, "noise.ppm", EWETexture::tType_simpleVert);
		printf("redwind value - %d \n", grassTextureID);

		std::string grassNames[5] = {
			"grassPatch0.obj",
			"grassPatch1.obj",
			"grassPatch2.obj",
			"grassPatch3.obj",
			"grassPatch4.obj",
		};

		//objectManager.grassField.resize(5, EWEGameObject::createGameObject());
		//printf("about to load model for grass field \n");

		
		for (int i = 0; i < 5; i++) {
			grassField[i] = EWEModel::createGrassModelFromFile(device, grassNames[i]);
		}

		std::array<std::vector<GrassInstance>, 5> instanceTranslation;
		glm::vec3 instanceRotations[4] = {
			//		cos	sin
			{0.f, 0.f, 0.f},
			{0.f, glm::half_pi<float>(), 0.f},
			{0.f, glm::pi<float>(), 0.f},
			{0.f, glm::quarter_pi<float>() * 3.f, 0.f}
		};

		//float grassRadius = 17.56f * (grassFieldWidth / 2);
		uint32_t grassPatchCount = 0;
		for (int i = 0; i < mapWidth * mapHeight; i++) {
			if (tiles.at(i) == 477) {
				grassPatchCount++;
			}
		}
		for (int i = 0; i < instanceTranslation.size(); i++) {
			instanceTranslation[i].reserve(grassPatchCount); // grassFieldWidth squared / 5
		}

		int counter = 0;
		int rowCounter = 0;
		int rotationCounter[5] = { 0,0,0,0,0 };
		TransformComponent instanceTransform{};
		for (int i = 0; i < mapWidth; i++) {
			counter = rowCounter;
			for (int j = 0; j < mapHeight; j++) {
				if (tiles.at(i + j * mapWidth) == TileFlag::TileFlag_solid) {
					//transform.translation.x = 0.5f * (static_cast<float>(currentTile % mapWidth) - (static_cast<float>(mapWidth) / 2.f));
					//transform.translation.z = static_cast<float>(0.5 * (std::floor(static_cast<double>(currentTile) / static_cast<double>(mapWidth)) - (static_cast<double>(mapHeight) / 2.0)));

					instanceTransform.translation.x = 0.5f * (static_cast<float>(i) - (static_cast<float>(mapWidth) / 2.f)) + 0.25f;
					instanceTransform.translation.z = 0.5f * (static_cast<float>(j) - (static_cast<float>(mapHeight) / 2.f)) + 0.25f;

					instanceTransform.rotation = instanceRotations[rotationCounter[counter]];
					instanceTranslation[counter].emplace_back(instanceTransform.mat4());

					rotationCounter[counter] = (rotationCounter[counter] + 1) % 4;
					//printf("counter value? : %d \n", counter);
				}
				else if (tiles.at(i + j * mapWidth) == TileFlag::TileFlag_solid) {
					tiles.at(i + j * mapWidth) = TileFlag::TileFlag_solid;
				}
				counter = (counter + 1) % 5;

				//printf("instance translation back - %.3f:%.3f:%.3f \n", instanceTranslation.back().x, instanceTranslation.back().y, instanceTranslation.back().z);
			}
			rowCounter = (rowCounter + 2) % 5;
		}

		//uint32_t totalPatchCount = 0;
		for (uint8_t i = 0; i < grassField.size(); i++) {
			//totalPatchCount += static_cast<uint32_t>(instanceTranslation[i].size());
			//printf("size of instancing buffer[%d] : %d \n", i, instanceTranslation[i].size() * sizeof(EWEModel::GrassInstance));
			if (instanceTranslation[i].size() == 0) {
				printf("no grass, not supported \n");
				throw std::runtime_error("no grass");
			}

			grassField[i]->AddInstancing(static_cast<uint32_t>(instanceTranslation[i].size()), sizeof(GrassInstance), instanceTranslation[i].data());
		}
	}

	void Level::enterLevelP(EWEDevice& device, std::string textureLocation, std::string tileMapLocation) {
		floorTextureID = EWETexture::addSceneTexture(device, textureLocation);
		tileSize = 32;

		loadGrass(device);
	}
	*/
	void Level::exitLevel() {
		tileMap.reset(nullptr);
		//floor.reset();
		//for (int i = 0; i < grassField.size(); i++) {
		//	grassField[i].reset();
		//}
		//tileMap->tileFlags.clear();
		backgroundTrees.clear();
	}
	void Level::loadBackTrees(EWEDevice& device) {
		TransformComponent transform{};

		backgroundTrees.reserve(backgroundTrans.size());
		backgroundTrees.emplace_back("treeTwo", device, false);
		transform.translation = backgroundTrans[0];
		backgroundTrees.back().transform = transform;
		backgroundTrees.back().drawable = true;
		for (int i = 1; i < backgroundTrans.size(); i++) {
			backgroundTrees.emplace_back(backgroundTrees[0]);
			transform.translation = backgroundTrans[i];
			backgroundTrees.back().transform = transform;
			backgroundTrees.back().drawable = true;
		}
	}
}