#include "ForestLevel.h"

#include "../pipelines/BillboardPipe.h"
#include "../pipelines/PipelineEnum.h"

namespace EWE {
	ForestLevel::ForestLevel(EWEDevice& device) : Level{"spookyForest.tmx", TileSet::TS_First} {
		exits.push_back(Level_Connector);

		TransformComponent entryTransform{};
		entryTransform.translation.x = -11.5f;
		entryTransform.translation.z = -2.f;
		entryTransform.rotation.y = -glm::half_pi<float>();
		entryPoints.emplace(Level::Level_Connector, entryTransform);

		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				treeData.emplace_back(false, glm::vec3{static_cast<float>(x) * 6.f, 0.f, static_cast<float>(y) * 6.f - 4.f});
			}
		}
		backgroundTrans = {
			{-13.f, 0.f, -5.f },
			{-13.f, 0.f, -7.f },
			{-13.f, 0.f, -9.f },
			{-13.f, 0.f, -11.f },
			{-13.f, 0.f, -13.f },

			{-11.f, 0.f, -13.f },
			{-9.f, 0.f, -13.f },
			{-7.f, 0.f, -13.f },
			{-5.f, 0.f, -13.f },
			{-3.f, 0.f, -13.f },
			{-1.f, 0.f, -13.f },
			{1.f, 0.f, -13.f },
			{3.f, 0.f, -13.f },
			{5.f, 0.f, -13.f },
			{7.f, 0.f, -13.f },
			{9.f, 0.f, -13.f },

			{11.f, 0.f, -13.f },
			{11.f, 0.f, -11.f },
			{11.f, 0.f, -9.f },
			{11.f, 0.f, -7.f },
			{11.f, 0.f, -5.f },
			{11.f, 0.f, -3.f },
			{11.f, 0.f, -1.f },
			{11.f, 0.f, 1.f },
			{11.f, 0.f, 3.f },
			{11.f, 0.f, 5.f },
			{11.f, 0.f, 7.f },
			{11.f, 0.f, 9.f },

			{-13.f, 0.f, 9.f },
			{-11.f, 0.f, 9.f },
			{-9.f, 0.f, 9.f },
			{-7.f, 0.f, 9.f },
			{-5.f, 0.f, 9.f },
			{-3.f, 0.f, 9.f },
			{-1.f, 0.f, 9.f },
			{1.f, 0.f, 9.f },
			{3.f, 0.f, 9.f },
			{5.f, 0.f, 9.f },
			{7.f, 0.f, 9.f },
			{9.f, 0.f, 9.f },
			{11.f, 0.f, 9.f },

			{-13.f, 0.f, 7.f },
			{-13.f, 0.f, 5.f },
			{-13.f, 0.f, 3.f },
			{-13.f, 0.f, 1.f },


		};
	}

	void ForestLevel::exitLevel() {
		auto materialHandler = RigidRenderingSystem::getRigidRSInstance();
		for (auto tree : trees) {
			for (auto treeTexIter = tree.ownedTextureIDs.begin(); treeTexIter != tree.ownedTextureIDs.end(); treeTexIter++) {
				materialHandler->removeByTransform(*treeTexIter, &tree.transform);
			}
			materialHandler->checkAndClearTextures();
			printf("removing tree pls \n");
		}
		trees.clear();
		Level::exitLevel();
		zero.reset();
		logs.clear();
		sticks.clear();
		
	}

	void ForestLevel::enterLevel(EWEDevice& device) {
		std::string textureLocation{ "WoodChop.png" };
		std::string tileMapLocation{ "models/WoodChop.tmx" };

		tileMap = std::make_unique<TileMap>(device, mapName, tileSetID);
		//enterLevelP(device, textureLocation, tileMapLocation);


		bool zeroTamed = (SaveJSON::saveData.petFlags & SaveJSON::PetFlags::PF_Zero) == SaveJSON::PF_Zero;
		if (!zeroTamed) {
			printf("loading zero into ForestLevel \n");
			zero = std::make_unique<Zero>(device);
			zero->giveSticks(&sticks);
		}
		else {
			printf("ZERO WAS NOT LOADED INTO FOREST LEVEL : %d \n", SaveJSON::saveData.petFlags);
		}

		trees.reserve(treeData.size());
		
		logTextureID = Texture_Builder::createSimpleTexture( "woodLog.png", false, false, VK_SHADER_STAGE_FRAGMENT_BIT);
		stickTextureID = Texture_Builder::createSimpleTexture( "stick.png", false, false, VK_SHADER_STAGE_FRAGMENT_BIT);

		glm::vec3 logTranslation{};
		for (int i = 0; i < treeData.size(); i++) {
			treeData[i].first = false;
			if (i == 0) {
				trees.emplace_back("treeTwo", device, false);
				trees.back().drawable = true;
			}
			else {
				trees.emplace_back(trees[0]);
				trees.back().drawable = true;
			}
			trees.at(i).transform.translation = treeData.at(i).second;
			logTranslation = trees.at(i).transform.translation;
			logTranslation.y += .25f;
			logs.emplace_back(logTranslation);
		}
		loadBackTrees(device);
	}

	void ForestLevel::render(FrameInfo const& frameInfo, float dt) {
		Level::render(frameInfo, dt);
		if (zero.get() != nullptr) {
			zero->renderUpdate();
		}

		bool pipeBinded = false;
		auto pipe = PipelineSystem::at(Pipe_billboard);
		PointLightPushConstants push{};
		for (int i = 0; i < logs.size(); i++) {
			if (logs[i].drawable) {
				if (!pipeBinded) {
					//bind pipe
					pipe->bindPipeline();
					pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameInfo.index));
					pipe->bindDescriptor(1, Texture_Manager::getDescriptorSet(logTextureID));
					pipeBinded = true;
					push.radius = 1.f;
				}
				push.position = glm::vec4(logs[i].translation, 1.f);
				pipe->pushAndDraw(&push);
			}
		}

		if(sticks.size() > 0){
			if (!pipeBinded) {
				pipe->bindPipeline();
				pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameInfo.index));
				pipeBinded = true;
				push.radius = 1.f;
			}
			pipe->bindDescriptor(1, Texture_Manager::getDescriptorSet(stickTextureID));
		}

		for (auto stick : sticks) {
			if (!stick.drawable) {
				continue;
			}
			push.position = glm::vec4(stick.translation, 1.f);
			pipe->pushAndDraw(&push);
		}

	}
	bool ForestLevel::pickLog(float x, float y) {
		for (int i = 0; i < logs.size(); i++) {
			if (!logs[i].drawable) {
				continue;
			}
			float horiDist = logs[i].translation.x - x;
			horiDist *= 1.f - (2.f * (horiDist < 0.f));
			float vertDist = logs[i].translation.z - y;
			vertDist *= 1.f - (2.f * (vertDist < 0.f));
			if ((horiDist < 0.5f) && (vertDist < 0.5f)) {
				logs[i].drawable = false;
				return true;
			}
		}
		return false;
	}
	void ForestLevel::dropStick(float x, float y, glm::vec2 forwardDir) {
		sticks.emplace_back(glm::vec3{ x + forwardDir.x, 0.25f, y - forwardDir.y} );
		sticks.back().drawable = true;
	}


	TileFlag ForestLevel::tileAt(float x, float y) {

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


		return tileMap->tileFlags.at(static_cast<int>(std::floor(x * 2.f)) + tileMap->width / 2 + static_cast<int>(std::floor(y * 2.f)) * tileMap->width + (tileMap->width * tileMap->height / 2));
	}

	bool ForestLevel::chopTree(glm::vec2 position, glm::vec2 direction) {

		for (int i = 0; i < treeData.size(); i++) {
			if (treeData[i].first) {
				continue;
			}

			glm::vec2 distanceVec{ trees[i].transform.translation.x - position.x, trees[i].transform.translation.z - position.y};
			float distance = glm::length(distanceVec);
			printf("distance : %.2f \n", distance);
			if (distance < 2.f) {
				distanceVec = glm::normalize(distanceVec);
				printf("direction : %.2f : %.2f \n", direction.x, direction.y);
				printf("incoming dot - %d : %.2f \n", i, glm::dot(distanceVec, direction));
				if (glm::dot(distanceVec, direction) > 0.5f) {
					trees[i].drawable = false;
					treeData[i].first = true;
					logs.at(i).drawable = true;
					return true;
				}
			}
		}
		return false;
	}
}