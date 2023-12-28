#pragma once

#include <EWEngine/graphics/model/EWE_Model.h>
#include <EWEngine/graphics/EWE_FrameInfo.h>
#include <EWEngine/graphics/PushConstants.h>
#include <EWEngine/systems/PipelineSystem.h>

#include "../pipelines/PipelineEnum.h"

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
		static std::unique_ptr<EWEModel> floor;

		Level() {
			
		}

		struct Level_Exit {
			LevelID destination;
			TransformComponent destinationSpawnPoint;
			Level_Exit(LevelID dest, TransformComponent& destSpawn) : destination{ dest }, destinationSpawnPoint{ destSpawn } {

			}
		};

		virtual void enterLevel(EWEDevice& device) = 0;
		virtual void exitLevel() {
			terrainObjects.clear();

		}

		std::vector<Level_Exit> exits;

		std::vector<Terrain_Object> terrainObjects{};

		TransformComponent floorTransform{};
		TextureID floorTextureID;

		void render(FrameInfo& frameInfo);

		
	};
}