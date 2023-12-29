#include "FirstLevel.h"

namespace EWE {
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
		for (int i = 0; i < treeData.size(); i++) {
			if (i == 0) {
				apples.emplace_back("apple", device, false);
				apples.back().drawable = false;
			}
			else {
				apples.emplace_back(apples[0]);
				apples.back().drawable = false;
			}
		}
	}
	void FirstLevel::exitLevel() {
		carrot.reset(nullptr);
		for (auto apple : apples) {

			auto materialHandler = MaterialHandler::getMaterialHandlerInstance();

			for (auto appleTexIter = apple.ownedTextureIDs.begin(); appleTexIter != apple.ownedTextureIDs.end(); appleTexIter++) {
				materialHandler->removeByTransform(*appleTexIter, &apple.transform);
			}
			materialHandler->checkAndClearTextures();
			printf("removing apple pls \n");
		}
		apples.clear();
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
		apples.at(treeID).transform.scale = glm::vec3{1.f, 1.f, 1.f};
		carrot->appleDropped(std::pair<uint8_t, glm::vec3>{treeID, treeData.at(treeID).second});
	}

	void FirstLevel::eatApple(uint8_t treeID) {
		printf("setting this mf to no draw \n");
		apples.at(treeID).drawable = false;
	}
}