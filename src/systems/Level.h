#pragma once

#include <EWEngine/graphics/model/EWE_Model.h>
#include <EWEngine/graphics/EWE_FrameInfo.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/systems/PipelineSystem.h>
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

#include "../pipelines/PipelineEnum.h"
#include "TileMap.h"
#include "SaveData.h"

#include <vector>
#include <memory>


namespace EWE {
	struct Terrain_Object {
		Terrain_Object(EWEDevice& device, std::string modelPath)
			:model{EWEModel::createModelFromFile(device, modelPath)}
		{

		}

		std::unique_ptr<EWEModel> model;

		struct Object_Instance {
			TransformComponent transform{};
			bool active{true};
		};

		std::unordered_map<TextureID, std::vector<Object_Instance>> objectTypes; //object types, keyed by texture

		void render(FrameInfo& frameInfo);
	};


	class Level {
	public:
		enum Level_Enum : LevelID {
			Level_Start,
			Level_First, //encounter with Carrot
			Level_Connector,
			Level_WoodChop,
			Level_SpookyForest,
		};

		Level(TileSet::TileSet_Enum tsEnum) : tileSet{tsEnum} {}

		struct Level_Exit {
			LevelID destination;
			TransformComponent destinationSpawnPoint;
			Level_Exit(LevelID dest, TransformComponent& destSpawn) : destination{ dest }, destinationSpawnPoint{ destSpawn } {

			}
		};

		virtual void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) = 0;
		virtual void exitLevel() {
			floor.reset();
			for (int i = 0; i < grassField.size(); i++) {
				grassField[i].reset();
			}
			tiles.clear();
		}

		std::vector<LevelID> exits{};

		//std::vector<Terrain_Object> terrainObjects{};

		TransformComponent floorTransform{};
		TextureID floorTextureID;

		//TileFlag tileFlagAt(float x, float y);

		virtual void render(FrameInfo& frameInfo);

		void loadGrass(EWEDevice& device);

		virtual TileFlag tileAt(float x, float y);

		TransformComponent const& getEntryTransform(LevelID fromLevel) {
			return entryPoints.at(fromLevel);
		}

	protected:
		std::unordered_map<LevelID, TransformComponent> entryPoints{};

		TileSet tileSet;
		std::unique_ptr<EWEModel> floor;
		std::vector<TileFlag> tiles;
		int32_t mapWidth;
		int32_t mapHeight;
		uint32_t tileSize;

		TextureID grassTextureID;
		float grassTime = 0.f;

		std::array<std::unique_ptr<EWEModel>, 5> grassField;

		

		void enterLevelP(EWEDevice& device, std::string textureLocation, std::string tileMapLocation);
	};
}