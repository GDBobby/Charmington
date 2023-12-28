#include "FirstLevel.h"

namespace EWE {
	void FirstLevel::enterLevel(EWEDevice& device) {
		floorTextureID = EWETexture::addSceneTexture(device, "firstArena.png");
		tileSize = 32;

		std::string tileMapLocation = "models/firstArena.tmx";

		std::ifstream inStream{ tileMapLocation };
		if (!inStream.is_open()) {
			if (!std::filesystem::exists(tileMapLocation)) {
				printf("loaded map doesn't exist : %s \n", tileMapLocation.c_str());
				throw std::runtime_error("inexistant file \n");
			}
			inStream.open(tileMapLocation);
			if (!inStream.is_open()) {
				printf("failed to load map tewice : %s \n", tileMapLocation.c_str());
				throw std::runtime_error("failed to loadm ap twice");
			}
		}
		inStream >> mapWidth;
		inStream >> mapHeight;

		floor = Basic_Model::generateSimple3DQuad(device);
		floorTransform.scale.x = static_cast<float>(mapWidth) / 2.f;
		floorTransform.scale.z = static_cast<float>(mapHeight) / 2.f;

		uint32_t tileCount = mapWidth * mapHeight;
		tiles.reserve(tileCount);
		//std::vector<TileInstance> instanceData;
		//instanceData.reserve(tileCount);
		//TransformComponent transform{};
		//transform.scale = { 0.505f, 0.5f, 0.505f };
		//glm::vec2 uvOffset;
		//uint32_t currentTile = 0;
		TileID tileID;
		while (!inStream.eof()) {
			//printf("reading from file : %d \n", currentTile);
			//transform.rotation.y = 0.f;
			//std::string tileString;
			//inStream >> tileString;
			//printf("tileString : %s \n", tileString.c_str());
			//continue;

			inStream >> tileID;
			tileSet.interpretTileID(tileID);// , transform.rotation.y);

			//transform.translation.x = 0.5f * (static_cast<float>(currentTile % mapWidth) - (static_cast<float>(mapWidth) / 2.f));
			//transform.translation.z = static_cast<float>(0.5 * (std::floor(static_cast<double>(currentTile) / static_cast<double>(mapWidth)) - (static_cast<double>(mapHeight) / 2.0)));

			//instanceData[currentTile].transform = transform.mat4();
			//tileSet.setUVOffset(tileID, uvOffset);
			tiles.emplace_back(tileID);
			//instanceData.emplace_back(transform.mat4(), uvOffset);
			//currentTile++;
		}

		loadGrass(device);


		//floor->AddInstancing(mapWidth * mapHeight, sizeof(TileInstance), instanceData.data());
	}
}