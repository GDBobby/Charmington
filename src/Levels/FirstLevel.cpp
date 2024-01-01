#include "FirstLevel.h"

namespace EWE {
	FirstLevel::FirstLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		exits.push_back(Level_Start);

		TransformComponent entryTransform{};
		entryTransform.translation.x = 9.f;

		entryPoints.emplace(Level::Level_Start, entryTransform);

		backgroundTrans = {
			glm::vec3{11.f, 0.f, -3.f},
			glm::vec3{11.f, 0.f, 4.f},
			glm::vec3{11.f, 0.f, 8.5f},
			glm::vec3{11.f, 0.f, -7.f},
			glm::vec3{7.f, 0.f, -9.5f},
			glm::vec3{4.f, 0.f, -9.5f},
			glm::vec3{1.f, 0.f, -9.5f},
			glm::vec3{-2.f, 0.f, -9.5f},
			glm::vec3{-5.f, 0.f, -9.5f},
			glm::vec3{-8.f, 0.f, -9.5f},
			glm::vec3{-8.f, 0.f, -9.5f},
			glm::vec3{-8.f, 0.f, -6.5f},
			glm::vec3{-8.f, 0.f, -3.5f},
			glm::vec3{-8.f, 0.f, -0.5f},
			glm::vec3{-8.f, 0.f, 3.5f},
			glm::vec3{-8.f, 0.f, 6.5f},
			glm::vec3{-8.f, 0.f, 9.5f},
			glm::vec3{7.f, 0.f, 9.5f},
			glm::vec3{4.f, 0.f, 9.5f},
			glm::vec3{1.f, 0.f, 9.5f},
			glm::vec3{-2.f, 0.f, 9.5f},
			glm::vec3{-5.f, 0.f, 9.5f},

		};
	}

	void FirstLevel::enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) {
		std::string textureLocation{ "firstArena.png" };
		std::string tileMapLocation{ "models/firstArena.tmx" };

		enterLevelP(device, textureLocation, tileMapLocation);
		printf("SaveJSON::saveData.petFlags : %lu \n", SaveJSON::saveData.petFlags);
		for (int i = 0; i < treeData.size(); i++) {
			treeData[i].first = false;
		}

		if ((SaveJSON::saveData.petFlags & SaveJSON::PF_Carrot) != SaveJSON::PF_Carrot) {
			printf("loading in carrot \n");
			carrot.reset(new Carrot(device, globalPool));
			TransformComponent transform{};

			carrot->setTransform(transform);
			std::vector<glm::vec3> treeLocations{};
			for (int i = 0; i < treeData.size(); i++) {
				treeLocations.push_back(treeData[i].second);
			}
			carrot->giveTreeLocations(treeLocations);
		}
		else {
			printf("NOT loading in carrot \n");
		}
		apples.reserve(treeData.size());
		trees.reserve(treeData.size());
		for (int i = 0; i < treeData.size(); i++) {
			if (i == 0) {
				apples.emplace_back("apple", device, false);
				trees.emplace_back("treeOne", device, false);
				trees.back().drawable = true;
				apples.back().drawable = false;
			}
			else {
				apples.emplace_back(apples[0]);
				apples.back().drawable = false;
				trees.emplace_back(trees[0]);
				trees.back().drawable = true;
			}
			trees.at(i).transform.translation = treeData.at(i).second;
		}
		loadBackTrees(device);
	}
	void FirstLevel::exitLevel() {
		carrot.reset(nullptr);
		auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
		for (auto apple : apples) {
			for (auto appleTexIter = apple.ownedTextureIDs.begin(); appleTexIter != apple.ownedTextureIDs.end(); appleTexIter++) {
				materialHandler->removeByTransform(*appleTexIter, &apple.transform);
			}
			materialHandler->checkAndClearTextures();
			printf("removing apple pls \n");
		}
		for (auto tree : trees) {
			for (auto treeTexIter = tree.ownedTextureIDs.begin(); treeTexIter != tree.ownedTextureIDs.end(); treeTexIter++) {
				materialHandler->removeByTransform(*treeTexIter, &tree.transform);
			}
			materialHandler->checkAndClearTextures();
			printf("removing tree pls \n");
		}
		trees.clear();
		apples.clear();
		Level::exitLevel();
	}
	void FirstLevel::render(FrameInfo& frameInfo) {
		Level::render(frameInfo);
		if (carrot.get() != nullptr) {
			carrot->renderUpdate();
		}
	}
	void FirstLevel::dropApple(EWEDevice& device, uint8_t treeID) {
		if (treeData.at(treeID).first == false) {
			treeData.at(treeID).first = true;
		}
		else {
			return;
		}


		apples.at(treeID).drawable = true;

		apples.at(treeID).transform.translation = treeData.at(treeID).second;
		apples.at(treeID).transform.translation.z += 2.f;
		apples.at(treeID).transform.scale = glm::vec3{1.f, 1.f, 1.f};

		carrot->appleDropped(std::pair<uint8_t, glm::vec3>{treeID, apples.at(treeID).transform.translation});
	}

	void FirstLevel::eatApple(uint8_t treeID) {
		printf("setting this mf to no draw \n");
		apples.at(treeID).drawable = false;
	}
	TileFlag FirstLevel::tileAt(float x, float y) {

		//printf("x ? %d \n", static_cast<int>(std::floor(x * 2.f)) + mapWidth / 2);
		//printf("y ? %d \n", static_cast<int>(std::floor(y * 2.f)) * mapHeight + (mapWidth * mapHeight / 2));
		for (auto& tree : trees) {

			//printf("tree position - x:y - %.2f:%.2f - %.2f:%.2f", tree.transform.translation.x, tree.transform.translation.z, x, y);

			float horiDist = tree.transform.translation.x - x;
			horiDist *= 1.f - (2.f * (horiDist < 0.f));
			float vertDist = tree.transform.translation.z - y;
			vertDist *= 1.f - (2.f * (vertDist < 0.f));


			//printf("tree dist sqrd : %.2f:%.2f \n", horiDist, vertDist);

			if ((horiDist < 1.f) && (vertDist < 1.f)) {
				return TileFlag::TileFlag_solid;
			}

		}
		

		return tiles.at(static_cast<int>(std::floor(x * 2.f)) + mapWidth / 2 + static_cast<int>(std::floor(y * 2.f)) * mapWidth + (mapWidth * mapHeight / 2));
	}
}