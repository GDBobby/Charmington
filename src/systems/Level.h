#pragma once

#include <EWEngine/graphics/model/EWE_Model.h>
#include <EWEngine/graphics/EWE_FrameInfo.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/systems/PipelineSystem.h>
#include <EWEngine/graphics/model/EWE_Basic_Model.h>

#include "../pipelines/PipelineEnum.h"
#include "TileMap.h"

#include <vector>
#include <memory>


typedef uint16_t LevelID;

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
		Level(TileSet::TileSet_Enum tsEnum) : tileSet{tsEnum} {}

		struct Level_Exit {
			LevelID destination;
			TransformComponent destinationSpawnPoint;
			Level_Exit(LevelID dest, TransformComponent& destSpawn) : destination{ dest }, destinationSpawnPoint{ destSpawn } {

			}
		};

		virtual void enterLevel(EWEDevice& device) = 0;
		virtual void exitLevel() {
			floor.reset();
			terrainObjects.clear();

		}

		std::vector<Level_Exit> exits;

		std::vector<Terrain_Object> terrainObjects{};

		TransformComponent floorTransform{};
		TextureID floorTextureID;

		TileFlag tileFlagAt(float x, float y);

		void render(FrameInfo& frameInfo);

		void loadGrass(EWEDevice& device);

	protected:
		TileSet tileSet;
		std::unique_ptr<EWEModel> floor;
		std::vector<TileID> tiles;
		int32_t mapWidth;
		int32_t mapHeight;
		uint32_t tileSize;

		TextureID grassTextureID;

		struct GrassPatch {
			TransformComponent transform;
			std::unique_ptr<EWEModel> model;
		};

		std::array<GrassPatch, 5> grassField;

		TileID tileAt(float x, float y);
	};
}