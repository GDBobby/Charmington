#include "ConnectorLevel.h"

#include <EWEngine/Systems/PipelineSystem.h>
#include "../pipelines/BackgroundPipe.h"
#include "../pipelines/PipelineEnum.h"

namespace EWE {
	ConnectorLevel::ConnectorLevel(EWEDevice& device) : Level{"connector.tmx", TileSet::TS_First } {
		exits.push_back(Level_Start);
		exits.push_back(Level_WoodChop);
		exits.push_back(Level_SpookyForest);

		TransformComponent entryTransform{};
		entryTransform.translation.x = 2.5f;
		entryTransform.translation.z = -1.f;
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Start, entryTransform);

		entryTransform.translation.x = 10.f;
		entryTransform.translation.y = 0.f;
		entryTransform.translation.z = -8.5f;

		entryTransform.rotation.y = 0.f;

		entryPoints.emplace(Level::Level_WoodChop, entryTransform);

		entryTransform.translation.x = 2.5f;
		entryTransform.translation.z = -16.f;
		entryTransform.rotation.y = 0.f;

		entryPoints.emplace(Level::Level_SpookyForest, entryTransform);

		backgroundTrans = {
			{0.f, 0.f, 1.f },
			{-3.f, 0.f, 1.f },
			{-6.f, 0.f, 1.f },
			{-6.f, 0.f, -2.f },
			{-6.f, 0.f, -5.f },
			{-6.f, 0.f, -8.f },
			{-6.f, 0.f, -11.f },
			{-6.f, 0.f, -14.f },
			{-6.f, 0.f, -17.f },
			{-3.f, 0.f, -17.f },
			{5.f, 0.f, -17.f },
			{8.f, 0.f, -17.f },
			{11.f, 0.f, -17.f },

			{11.f, 0.f, 1.f },
			{11.f, 0.f, -2.f },
			{11.f, 0.f, -5.f },
			{11.f, 0.f, -11.f },
			{11.f, 0.f, -14.f },
			{8.f, 0.f, 1.f },
			{5.f, 0.f, 1.f },

		};
	}
	void ConnectorLevel::exitLevel() {
		//auto materialHandler = RigidRenderingSystem::getRigidRSInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		//rock.reset();
		extension.clear();
		waterModel.reset();
		extensionModel.reset();
		sheet.reset();
		Level::exitLevel();
		scareCount = 0;

		planks.clear();
		//placedPlankCount = 0;
	}

	void ConnectorLevel::enterLevel(EWEDevice& device) {
		std::string textureLocation{ "connector.png" };
		std::string tileMapLocation{ "models/connector.tmx" };

		tileMap = std::make_unique<TileMap>(device, mapName, tileSetID);
		//enterLevelP(device, textureLocation, tileMapLocation);
		for (int i = 0; i < waterTextures.size(); i++) {
			std::string waterTextureLocation{ "water" + std::to_string(i) + ".png" };
			waterTextures[i] = Texture_Builder::createSimpleTexture( waterTextureLocation, false, false, VK_SHADER_STAGE_FRAGMENT_BIT);
		}

		waterTransform.scale.x = static_cast<float>(tileMap->width) / 2.f;
		waterTransform.scale.z = 6.f;
		waterTransform.translation.y = -0.01f;
		waterTransform.translation.z = -20.f;
		waterModel = Basic_Model::generateSimple3DQuad(device, glm::vec2{ static_cast<float>(tileMap->width), 12.f });
		extensionModel = Basic_Model::generateSimple3DQuad(device);
		builtBridge = (SaveJSON::saveData.obstacleFlags & SaveJSON::ObstacleFlags::OF_Bridge) == SaveJSON::ObstacleFlags::OF_Bridge;
		loadExtension(device);
		//rock.reset(new EweObject("rock", device, false));
		//rock->transform.translation.z = -11.f;
		scaredSheet = (SaveJSON::saveData.obstacleFlags & SaveJSON::ObstacleFlags::OF_Sheet) == SaveJSON::ObstacleFlags::OF_Sheet;
		if (!scaredSheet) {
			sheet = std::make_unique<Sheet>(device);
			sheet->transform.translation.x = 0.5f;
			sheet->transform.translation.z = -20.f;
			sheet->guarding = true;
		}

		planks.reserve(plankTranslations.size());
		planks.emplace_back("plank", device, false);
		planks.back().drawable = false;
		planks.back().transform.translation = plankTranslations[0];
		planks.back().transform.rotation = glm::vec3{ 0.f, glm::half_pi<float>(), 0.f };
		//planks.back().transform.rotation = glm::vec3{ glm::half_pi<float>(), glm::half_pi<float>(), 0.f };
		if (!builtBridge) {
			if (placedPlankCount > 0) {
				planks.front().drawable = true;
			}

			for (int i = 1; i < placedPlankCount; i++) {

				planks.emplace_back(planks.front());
				planks.back().transform.translation = plankTranslations.at(i);
				planks.back().drawable = true;
				planks.back().transform.rotation = glm::vec3{ 0.f, glm::half_pi<float>(), 0.f };
				//planks.back().transform.rotation = glm::vec3{ glm::half_pi<float>(), glm::half_pi<float>(), 0.f };
			}
		}
		else {
			planks.front().drawable = true;
			for (int i = 1; i < plankTranslations.size(); i++) {

				planks.emplace_back(planks.front());
				planks.back().transform.translation = plankTranslations.at(i);
				planks.back().drawable = true;
				planks.back().transform.rotation = glm::vec3{ 0.f, glm::half_pi<float>(), 0.f };
				//planks.back().transform.rotation = glm::vec3{ glm::half_pi<float>(), glm::half_pi<float>(), 0.f };
			}
		}
		loadBackTrees(device);
	}
	void ConnectorLevel::render(FrameInfo const& frameInfo, float dt) {
		//printf("water time : %.2f \n", waterTime);
		waterTime = glm::mod(waterTime + dt, glm::half_pi<float>());

		uint32_t waterIndex = static_cast<uint8_t>(std::floor(waterTime / glm::quarter_pi<float>()));

		Level::render(frameInfo, dt);
		
		PipelineSystem::at(Pipe_background)->bindDescriptor(1, Texture_Manager::getDescriptorSet(waterTextures[waterIndex]));
		
		PipelineSystem::at(Pipe_background)->bindModel(waterModel.get());

		ModelPushData push;
		push.modelMatrix = waterTransform.mat4();

		PipelineSystem::at(Pipe_background)->pushAndDraw(&push);

		PipelineSystem::at(Pipe_background)->bindModel(extensionModel.get());
		push.modelMatrix = extensionTransform.mat4();

		PipelineSystem::at(Pipe_background)->bindDescriptor(1, Texture_Manager::getDescriptorSet(extensionTexture));
		PipelineSystem::at(Pipe_background)->pushAndDraw(&push);
		if (sheet.get() != nullptr) {
			sheet->renderUpdate();
		}
	}

	void ConnectorLevel::loadExtension(EWEDevice& device) {
		std::string textureLocation{ "connectorExtension.png" };
		std::string tileMapLocation{ "models/connectorExtension.tmx" };

		extensionTexture = Texture_Builder::createSimpleTexture( textureLocation, false, false, VK_SHADER_STAGE_FRAGMENT_BIT);

		std::ifstream inStream{ tileMapLocation };
		if (!inStream.is_open()) {
			if (!std::filesystem::exists(tileMapLocation)) {
				printf("loaded map doesn't exist : %s \n", tileMapLocation.c_str());
				throw std::runtime_error("inexistant file \n");
			}
			inStream.open(tileMapLocation);
			if (!inStream.is_open()) {
				printf("failed to load map twice : %s \n", tileMapLocation.c_str());
				throw std::runtime_error("failed to load map twice");
			}
		}
		inStream >> extensionWidth;
		inStream >> extensionHeight;

		extensionTransform.translation.z = -24.f;// ?
		extensionTransform.scale.x = static_cast<float>(extensionWidth) / 2.f;
		extensionTransform.scale.z = -static_cast<float>(extensionHeight) / 2.f;

		uint32_t tileCount = extensionWidth * extensionHeight;
		extension.reserve(tileCount + (6 * tileMap->width));
		uint16_t tileBuffer;
		while (!inStream.eof()) {
			inStream >> tileBuffer;
			extension.emplace_back((TileFlag)tileBuffer);
		}
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < tileMap->width; x++) {
				if (builtBridge && x >= 53 && x <= 56) {
					extension.emplace_back(TileFlag::TileFlag_none);
				}
				else {
					extension.emplace_back(TileFlag::TileFlag_solid);
				}
			}
		}
		//tiles.insert(tiles.begin(), extension.begin(), extension.end());
	}

	TileFlag ConnectorLevel::tileAt(float x, float y) {
		//printf("overriden tile at \n");
		//printf("x ? %d \n", static_cast<int>(std::floor(x * 2.f + 1.f)) + mapWidth / 2);
		//printf("y ? %d \n", static_cast<int>(std::floor(y * 2.f + 10.f)) * mapWidth + (mapWidth * (mapHeight)));

		return tileMap->tileFlags.at(static_cast<int>(std::floor(x * 2.f + 1.f)) + tileMap->width / 2 + static_cast<int>(std::floor(y * 2.f + 12.f)) * tileMap->width + (tileMap->width * (tileMap->height / 2)));
	}

	void ConnectorLevel::bark(float x, float y) {
		if (sheet.get() != nullptr) {
			if (sheet->scaredAnim()) {
				return;
			}

			float sheetDist = (sheet->transform.translation.x - x) * (sheet->transform.translation.x - x) + (sheet->transform.translation.z - y) * (sheet->transform.translation.z - y);
			printf("bork sheet dist : %.2f \n", sheetDist);
			if (sheetDist < 5.f) {
				scareCount++;
				sheet->scare();
				printf("scare count : %d \n", scareCount);
				if (scareCount >= 3) {
					scaredSheet = true;
					sheet->flee();
					//SaveJSON::saveData.obstacleFlags |= SaveJSON::ObstacleFlags::OF_Sheet;
					//return true;
				}
			}
		}
		//return false;
	}

	bool ConnectorLevel::dropPlank(float x, float y, glm::vec2 forwardDir) {
		if (builtBridge) {
			return false;
		}

		if (y < -18.5f) {


			printf("drop plank dot prod : %.2f \n", glm::dot(forwardDir, glm::vec2{ 0.f, -1.f }));
			if (glm::dot(forwardDir, glm::vec2{ 0.f, -1.f }) > 0.5f) {
				printf("plaicng plank \n");

				if ((placedPlankCount > 0)) {
					planks.emplace_back(planks.front());
					planks.back().transform.translation = plankTranslations.at(placedPlankCount);
					planks.back().drawable = true;
					planks.back().transform.rotation = glm::vec3{ 0.f, glm::half_pi<float>(), 0.f };
					placedPlankCount++;
				}
				else {
					placedPlankCount++;
					planks.at(0).drawable = true;
				}
				if (placedPlankCount == plankTranslations.size()) {
					printf("finished bridge construction \n");
					builtBridge = true;
					for (int y = 5; y < 12; y++) {
						uint32_t tileIter = y * tileMap->width;
						for (int x = 53; x <= 56; x++) {

							tileMap->tileFlags.at(y * tileMap->width + x) = TileFlag::TileFlag_none;
							SaveJSON::saveData.obstacleFlags |= SaveJSON::OF_Bridge;
							SaveJSON::saveToJsonFile();
						}
					}
				}
				return true;
			}
		}
		return false;
	}
}