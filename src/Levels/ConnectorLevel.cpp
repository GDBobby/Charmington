#include "ConnectorLevel.h"

namespace EWE {
	ConnectorLevel::ConnectorLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_Start);
		exits.push_back(Level_WoodChop);
		exits.push_back(Level_Fourth);

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

		entryPoints.emplace(Level::Level_Fourth, entryTransform);


	}
	void ConnectorLevel::exitLevel() {
		//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		//rock.reset();
		extension.clear();
		waterModel.reset();
		extensionModel.reset();
		Level::exitLevel();

	}

	void ConnectorLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "connector.png" };
		std::string tileMapLocation{ "models/connector.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
		for (int i = 0; i < waterTextures.size(); i++) {
			std::string waterTextureLocation{ "water" + std::to_string(i) + ".png" };
			waterTextures[i] = EWETexture::addSceneTexture(device, waterTextureLocation);
		}

		waterTransform.scale.x = static_cast<float>(mapWidth) / 2.f;
		waterTransform.scale.z = 6.f;
		waterTransform.translation.y = -0.01f;
		waterTransform.translation.z = -20.f;
		waterModel = Basic_Model::generateSimple3DQuad(device, glm::vec2{ static_cast<float>(mapWidth), 12.f });
		extensionModel = Basic_Model::generateSimple3DQuad(device);
		loadExtension(device);
		//rock.reset(new EweObject("rock", device, false));
		//rock->transform.translation.z = -11.f;
	}
	void ConnectorLevel::render(FrameInfo& frameInfo) {
		waterTime += frameInfo.time;
		//printf("water time : %.2f \n", waterTime);
		waterTime = glm::mod(waterTime, glm::half_pi<float>());

		uint32_t waterIndex = static_cast<uint8_t>(std::floor(waterTime / glm::quarter_pi<float>()));

		Level::render(frameInfo);

		PipelineSystem::at(Pipe_background)->bindDescriptor(1, EWETexture::getDescriptorSets(waterTextures[waterIndex], frameInfo.cmdIndexPair.second));
		
		PipelineSystem::at(Pipe_background)->bindModel(waterModel.get());

		ModelPushData push;
		push.modelMatrix = waterTransform.mat4();

		PipelineSystem::at(Pipe_background)->pushAndDraw(&push);

		PipelineSystem::at(Pipe_background)->bindModel(extensionModel.get());
		push.modelMatrix = extensionTransform.mat4();

		PipelineSystem::at(Pipe_background)->bindDescriptor(1, EWETexture::getDescriptorSets(extensionTexture, frameInfo.cmdIndexPair.second));
		PipelineSystem::at(Pipe_background)->pushAndDraw(&push);
	}

	void ConnectorLevel::loadExtension(EWEDevice& device) {
		std::string textureLocation{ "connectorExtension.png" };
		std::string tileMapLocation{ "models/connectorExtension.tmx" };

		extensionTexture = EWETexture::addSceneTexture(device, textureLocation);

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
		extension.reserve(tileCount + (6 * mapWidth));
		uint16_t tileBuffer;
		while (!inStream.eof()) {
			inStream >> tileBuffer;
			extension.emplace_back((TileFlag)tileBuffer);
		}
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < mapWidth; x++) {
				extension.emplace_back(TileFlag::TileFlag_none);
			}
		}
		tiles.insert(tiles.begin(), extension.begin(), extension.end());
	}

	TileFlag ConnectorLevel::tileAt(float x, float y) {
		//printf("overriden tile at \n");
		//printf("x ? %d \n", static_cast<int>(std::floor(x * 2.f + 1.f)) + mapWidth / 2);
		//printf("y ? %d \n", static_cast<int>(std::floor(y * 2.f + 10.f)) * mapWidth + (mapWidth * (mapHeight)));

		return tiles.at(static_cast<int>(std::floor(x * 2.f + 1.f)) + mapWidth / 2 + static_cast<int>(std::floor(y * 2.f + 12.f)) * mapWidth + (mapWidth * (mapHeight / 2)));
	}
}