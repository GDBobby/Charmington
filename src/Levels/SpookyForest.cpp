#include "SpookyForest.h"
#include <EWEngine/Sound_Engine.h>
#include "../MusicEnum.h"

namespace EWE {
	SpookyForest::SpookyForest(EWEDevice& device) : Level{"spookyForest.tmx", TileSet::TS_First} {
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.translation.x = 0.f;
		entryTransform.translation.z = 3.5f;
		entryTransform.rotation.y = -glm::quarter_pi<float>() * 3.f;
		entryPoints.emplace(Level::Level_Connector, entryTransform);

		treeData.emplace_back(false, glm::vec3{ -3.f, 0.f, -3.5f });
		treeData.emplace_back(false, glm::vec3{ 4.f, 0.f, 1.5f });
	}

	void SpookyForest::exitLevel() {
		auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		for (auto tree : trees) {
			for (auto treeTexIter = tree.ownedTextureIDs.begin(); treeTexIter != tree.ownedTextureIDs.end(); treeTexIter++) {
				materialHandler->removeByTransform(*treeTexIter, &tree.transform);
			}
			materialHandler->checkAndClearTextures();
			printf("removing tree pls \n");
		}
		trees.clear();
		Level::exitLevel();
		sheet.reset();
		rock.reset();

		SoundEngine::getSoundEngineInstance()->stopMusic();
		SoundEngine::getSoundEngineInstance()->playMusic(Music_forest, true);
	}


	void SpookyForest::enterLevel(EWEDevice& device) {
		std::string textureLocation{ "SpookyForest.png" };
		std::string tileMapLocation{ "models/SpookyForest.tmx" };

		tileMap = std::make_unique<TileMap>(device, mapName, tileSetID);
		//enterLevelP(device, textureLocation, tileMapLocation);
		sheet = std::make_unique<Sheet>(device);
		sheet->transform.translation = treeData[1].second;
		sheet->transform.translation.y += 1.f;

		SoundEngine::getSoundEngineInstance()->stopMusic();
		SoundEngine::getSoundEngineInstance()->playMusic(Music_spookyForest, true);

		rock = std::make_unique<EweObject>("rock", device, false);
		rock->transform.translation = glm::vec3{ 0.f, 0.f, -1.f };
		rock->drawable = true;
		trees.reserve(treeData.size());
		
		trees.emplace_back("treeTwo", device, false);
		trees.back().transform.translation = treeData[0].second;
		trees.back().drawable = true;
		treeData[0].first = false;
		for (int i = 1; i < treeData.size(); i++) {
			treeData[i].first = false;
			trees.emplace_back(trees[0]);
			trees.back().transform.translation = treeData[i].second;
			trees.back().drawable = true;
		}
	}

	void SpookyForest::render(FrameInfo& frameInfo) {
		Level::render(frameInfo);
		sheet->renderUpdate();
	}
	bool SpookyForest::chopTree(glm::vec2 position, glm::vec2 direction) {

		for (int i = 0; i < treeData.size(); i++) {
			if (treeData[i].first) {
				continue;
			}

			glm::vec2 distanceVec{ trees[i].transform.translation.x - position.x, trees[i].transform.translation.z - position.y };
			float distance = glm::length(distanceVec);
			printf("distance : %.2f \n", distance);
			if (distance < 2.f) {
				distanceVec = glm::normalize(distanceVec);
				printf("direction : %.2f : %.2f \n", direction.x, direction.y);
				printf("incoming dot - %d : %.2f \n", i, glm::dot(distanceVec, direction));
				if (glm::dot(distanceVec, direction) > 0.5f) {
					trees[i].drawable = false;
					treeData[i].first = true;
					float ghostDist = (trees[i].transform.translation.x - sheet->transform.translation.x) * (trees[i].transform.translation.x - sheet->transform.translation.x) +
						(trees[i].transform.translation.z - sheet->transform.translation.z) * (trees[i].transform.translation.z - sheet->transform.translation.z);
					if (ghostDist < 1.f) {
						bool foundTree = false;
						for (int j = 0; j < treeData.size(); j++) {
							if (treeData[j].first) {
								continue;
							}
							foundTree = true;
							sheet->GoTo(treeData[j].second);
							break;
						}
						if (!foundTree) {
							sheet->GoTo(glm::vec3{ rock->transform.translation.x, 2.f, rock->transform.translation.z });
						}
					}


					return true;
				}
			}
		}
		return false;
	}
	TileFlag SpookyForest::tileAt(float x, float y) {

		//printf("x ? %d \n", static_cast<int>(std::floor(x * 2.f)) + mapWidth / 2);
		//printf("y ? %d \n", static_cast<int>(std::floor(y * 2.f)) * mapHeight + (mapWidth * mapHeight / 2));
		for (int i = 0; i < treeData.size(); i++) {
			if (treeData[i].first) {
				continue;
			}
			//printf("tree position - x:y - %.2f:%.2f - %.2f:%.2f", tree.transform.translation.x, tree.transform.translation.z, x, y);

			float horiDist = trees[i].transform.translation.x - x;
			horiDist *= 1.f - (2.f * (horiDist < 0.f));
			float vertDist = trees[i].transform.translation.z - y;
			vertDist *= 1.f - (2.f * (vertDist < 0.f));


			//printf("tree dist sqrd : %.2f:%.2f \n", horiDist, vertDist);

			if ((horiDist < 1.f) && (vertDist < 1.f)) {
				return TileFlag::TileFlag_solid;
			}

		}
		if ((x <= rockCollBox[0]) && (x >= rockCollBox[2]) && (y >= rockCollBox[1]) && (y <= rockCollBox[3])) {
			return TileFlag::TileFlag_solid;
		}


		return tileMap->tileFlags.at(static_cast<int>(std::floor(x * 2.f)) + tileMap->width / 2 + static_cast<int>(std::floor(y * 2.f)) * tileMap->width + (tileMap->width * tileMap->height / 2));
	}
	void SpookyForest::bark(float x, float y) {
		if (sheet.get() != nullptr) {
			printf("barking in spooky forest \n");
			if (sheet->animState == SheetSkeleton::Anim_idle) {
				float rockDist = (rock->transform.translation.x - sheet->transform.translation.x) * (rock->transform.translation.x - sheet->transform.translation.x) +
					(rock->transform.translation.z - sheet->transform.translation.z) * (rock->transform.translation.z - sheet->transform.translation.z);

				if (rockDist < 0.5f) {
					sheet->GoTo({ 0.f, 1.f, -9.f });
				}
			}
		}
		//return false;
	}
}