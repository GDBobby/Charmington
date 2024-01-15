#include "CharmerOverlay.h"

#include <EWEngine/Graphics/Texture.h>
#include <EWEngine/Graphics/Frame_Info.h>

//need to replace this include with local texture enum
#include <EWEngine/GUI/MenuModule.h>

#include "../Systems/SaveData.h"

namespace EWE {


	CharmerOverlay::CharmerOverlay(EWEDevice& device, float screenWidth, float screenHeight,  uint8_t& logCount) : OverlayBase{ screenWidth, screenHeight }, logCount{ logCount } {
		isActive = false;
		gameObjects.reserve(objects_size);
		//glm::ivec2 screenScale{};
		//screenScale.x =  
		//screenScale.y = -static_cast<int>(screenHeight);
		//thats pixels
		glm::vec2 tileScale{ 1.f / 12.f, 1 / 8.f};
		//UIComp::convertScreenTo2D(screenScale, tileScale, screenWidth, screenHeight);

		for (int i = 0; i < objects_size; i++) {
			gameObjects.emplace_back(GameObject2D::createGameObject());
			gameObjects.back().transform2d.scale = tileScale;
			gameObjects.back().transform2d.translation.x = -.875f;
			gameObjects.back().transform2d.translation.y = -.775f + (i * .2f);

			overlayBackground.emplace_back();
			overlayBackground.back().scale = glm::vec2{ 1.f / 6.f, 1.f / 6.f };
			overlayBackground.back().translation.y = gameObjects.back().transform2d.translation.y;
			overlayBackground.back().translation.x = -.85f;
			//overlayBackground.back().translation.x f;
		}
		gameObjects.at(0).drawable = logCount > 0;

		tamedCarrot = (SaveJSON::saveData.petFlags & SaveJSON::PF_Carrot) == SaveJSON::PF_Carrot;
		gameObjects.at(1).drawable = tamedCarrot;
		tamedZero = (SaveJSON::saveData.petFlags & SaveJSON::PF_Zero) == SaveJSON::PF_Zero;
		gameObjects.at(2).drawable = tamedZero;

		overlayBackID = EWETexture::addGlobalTexture(device, "OverlayBack.png");
		objectTextures.reserve(gameObjects.size());
		objectTextures.emplace_back(EWETexture::addGlobalTexture(device, "woodPlank.png"));
		//printf("wood log tex ID : %d \n", objectTextures.back());
		objectTextures.emplace_back(EWETexture::addGlobalTexture(device, "carrotFace.png"));
		objectTextures.emplace_back(EWETexture::addGlobalTexture(device, "zeroFace.png"));
		//printf("carrot face tex ID : %d \n", objectTextures.back());


		/*
		enum CharmerString_Enum {
			BS_logCount,
			BS_carrotKB,
		};
		*/
		textStructs.resize(BS_size);
		textStructs[BS_logCount].string = "E"; 
		textStructs[BS_logCount].string += std::to_string(logCount);
		textStructs[BS_logCount].x = screenWidth * 0.1f;
		textStructs[BS_logCount].y = screenHeight * 0.1f;
		textStructs[BS_logCount].align = TA_center;
		textStructs[BS_logCount].scale = 2.f;

		textStructs[BS_carrotKB].string = "Q";
		textStructs[BS_carrotKB].x = screenWidth * 0.1f;
		textStructs[BS_carrotKB].y = screenHeight * 0.2f;
		textStructs[BS_carrotKB].align = TA_center;
		textStructs[BS_carrotKB].scale = 2.f;

		textStructs[BS_zeroKB].string = "R";
		textStructs[BS_zeroKB].x = screenWidth * 0.1f;
		textStructs[BS_zeroKB].y = screenHeight * 0.3f;
		textStructs[BS_zeroKB].align = TA_center;
		textStructs[BS_zeroKB].scale = 2.f;

	}

	void CharmerOverlay::SetDrawState(bool drawState) {
		isActive = drawState;

	}

	void CharmerOverlay::drawObjects(std::pair<VkCommandBuffer, uint8_t> cmdIndexPair) {
		//std::cout << "DRAWING BATTLE OVERLAY \n";
		if (isActive) {
			//printf("game ui is active \n");
			Simple2DPushConstantData push{};
			push.color = glm::vec3(1.f);

			gameObjects.at(logs).drawable = logCount > 0;


			for (int i = 0; i < gameObjects.size(); i++) {
				if (!gameObjects[i].drawable) {
					continue;
				}
				//printf("binding tex ID : %d:%d \n", objectTextures[i], i);
				Dimension2::bindTexture2D(objectTextures[i]);

				//printf("after binding tex ID : %d \n", i);

				push.scaleOffset = glm::vec4(gameObjects[i].transform2d.scale, gameObjects[i].transform2d.translation);
				Dimension2::pushAndDraw(push);
			}
			Dimension2::bindTexture2D(overlayBackID);
			for (int i = 0; i < gameObjects.size(); i++) {
				if (!gameObjects[i].drawable) {
					continue;
				}
				push.scaleOffset = glm::vec4(overlayBackground[i].scale, overlayBackground[i].translation);
				Dimension2::pushAndDraw(push);
			}
		}
	}

	void CharmerOverlay::drawText() {
		if (isActive) {
			if (logCount > 0) {
				textStructs[BS_logCount].string = "E";
				textStructs[BS_logCount].string += std::to_string(logCount);
				TextOverlay::staticAddText(textStructs[BS_logCount]);
			}
			else {
				gameObjects.at(BS_logCount).drawable = false;
			}
			if (tamedCarrot) {
				TextOverlay::staticAddText(textStructs[BS_carrotKB]);
			}
			if (tamedZero) {
				TextOverlay::staticAddText(textStructs[BS_zeroKB]);
			}
		}
	}
	void CharmerOverlay::updateFromSettings() {

		tamedCarrot = (SaveJSON::saveData.petFlags & SaveJSON::PF_Carrot) == SaveJSON::PF_Carrot;
		gameObjects.at(1).drawable = tamedCarrot;
		tamedZero = (SaveJSON::saveData.petFlags & SaveJSON::PF_Zero) == SaveJSON::PF_Zero;
		gameObjects.at(2).drawable = tamedZero;
		

		gameObjects.at(0).drawable = logCount > 0;

	}
}