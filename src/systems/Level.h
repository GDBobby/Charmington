#pragma once

#include <EWEngine/Graphics/Model/Model.h>
#include <EWEngine/Graphics/PushConstants.h>
#include <EWEngine/Graphics/Model/Basic_Model.h>
#include <EWEngine/Graphics/EWE_Object.h>

#include "TileMap.h"
#include "SaveData.h"

#include <vector>
#include <memory>


namespace EWE {
	struct Billboard {
		glm::vec3 translation;
		bool drawable = false;

		Billboard() : translation{} {}
		Billboard(glm::vec3 const& translation) : translation{ translation } {
		}
	};

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

		struct Level_Exit {
			LevelID destination;
			TransformComponent destinationSpawnPoint;
			Level_Exit(LevelID dest, TransformComponent& destSpawn) : destination{ dest }, destinationSpawnPoint{ destSpawn } {

			}
		};

		Level(std::string mapName, TileSet::TileSet_Enum tileSetID) : mapName{ mapName }, tileSetID{tileSetID} {}

		virtual void enterLevel(EWEDevice& device) = 0;
		virtual void exitLevel();

		std::vector<LevelID> exits{};

		//std::vector<Terrain_Object> terrainObjects{};

		TransformComponent floorTransform{};

		//TileFlag tileFlagAt(float x, float y);

		virtual void render(FrameInfo const& frameInfo, float dt);

		//void loadGrass(EWEDevice& device);

		virtual TileFlag tileAt(float x, float y);

		TransformComponent const& getEntryTransform(LevelID fromLevel) {
			return entryPoints.at(fromLevel);
		}

	protected:
		std::unordered_map<LevelID, TransformComponent> entryPoints{};

		std::unique_ptr<EWEModel> floor;

		std::string mapName; 
		TileSet::TileSet_Enum tileSetID;
		std::unique_ptr<TileMap> tileMap{nullptr};


		std::vector<EweObject> backgroundTrees{};

		//void enterLevelP(EWEDevice& device, std::string textureLocation, std::string tileMapLocation);
		std::vector<glm::vec3> backgroundTrans{};
		void loadBackTrees(EWEDevice& device);
	};
}