#include "StartArea.h"

namespace EWE {
	StartLevel::StartLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_First);
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Start, entryTransform);

		entryTransform.translation.x = -10.f;
		entryTransform.translation.y = 0.f;
		entryTransform.translation.z = -0.5f;

		entryTransform.rotation.y = 0.f;

		entryPoints.emplace(Level::Level_First, entryTransform);

		entryTransform.translation.x = 0.f;
		entryTransform.translation.y = 0.f;
		entryTransform.translation.z = -10.0f;

		entryPoints.emplace(Level::Level_Connector, entryTransform);

		backgroundTrans = {
			glm::vec3{4.f, 0.f, -12.f},
			glm::vec3{-4.f, 0.f, -12.f},
			glm::vec3{8.f, 0.f, -12.f},
			glm::vec3{-8.f, 0.f, -12.f},
			glm::vec3{12.f, 0.f, -12.f},
			glm::vec3{-12.f, 0.f, -12.f},
			glm::vec3{12.f, 0.f, -7.f},
			glm::vec3{-12.f, 0.f, -7.f},
			glm::vec3{12.f, 0.f, -3.f},
			glm::vec3{-12.f, 0.f, -3.f},
			glm::vec3{12.f, 0.f, 4.f},
			glm::vec3{-12.f, 0.f, 4.f},
			glm::vec3{12.f, 0.f, 10.f},
			glm::vec3{-12.f, 0.f, 10.f},

		};
	}
	void StartLevel::exitLevel() {
		//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
		//	materialHandler->removeByTransform(*iter, &rock->transform);
		//}
		//rock.reset();
		Level::exitLevel();
		stumps.clear();
	}

	bool StartLevel::chopTree(glm::vec2 position, glm::vec2 direction) {
		if ((position.x > -1.5f) && (position.x < 1.5f)) {

			for (int i = 0; i < stumpPositions.size(); i++) {
				glm::vec2 distanceVec = stumpPositions[i] - position;
				float distance = glm::length(distanceVec);
				printf("distance : %.2f \n", distance);
				if (distance < 2.f) {
					distanceVec = glm::normalize(distanceVec);
					printf("direction : %.2f : %.2f \n", direction.x, direction.y);
					printf("incoming dot - %d : %.2f \n", i, glm::dot(distanceVec, direction));
					if (glm::dot(distanceVec, direction) > 0.5f) {
						for (int j = 0; j < stumps.size(); j++) {
							stumps[j].drawable = false;
						}
						for (int j = 0; j < 6; j++) {
							tiles.at(47 + 29 * mapWidth + j) = TileFlag_none;
						}
						SaveJSON::saveData.obstacleFlags |= SaveJSON::OF_Stump;
						return true;
					}
				}
			}
		}
		return false;

	}

	void StartLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "startArea.png" };
		std::string tileMapLocation{ "models/startArea.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
		bool stumpsActive = (SaveJSON::saveData.obstacleFlags & SaveJSON::OF_Stump) == 0;

		stumps.reserve(2);
		stumps.emplace_back("stump", device, false);
		stumps.back().transform.translation.z = -11.f;
		stumps.back().transform.translation.x = -1.f;
		stumps.back().transform.rotation.x = glm::half_pi<float>();
		stumps.back().drawable = stumpsActive;
		stumps.emplace_back(stumps[0]);
		stumps.back().transform.translation.z = -11.f;
		stumps.back().transform.translation.x = 1.f;
		stumps.back().transform.rotation.x = glm::half_pi<float>();
		stumps.back().drawable = stumpsActive;

		if (stumpsActive) {
			for (int i = 0; i < 6; i++) {
				tiles.at(47 + 29 * mapWidth + i) = TileFlag_solid;
			}
		}
		
		loadBackTrees(device);

	}
}