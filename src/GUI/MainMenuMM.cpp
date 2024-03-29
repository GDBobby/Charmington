#include "MainMenuMM.h"

#include "EWEngine/Graphics/Texture/Texture_Manager.h"

namespace EWE {
	MainMenuMM::MainMenuMM(EWEDevice& device, float screenWidth, float screenHeight) {
		float widthRescaling = screenWidth / DEFAULT_WIDTH;
		float heightRescaling = screenHeight / DEFAULT_HEIGHT;

		//labels.emplace_back("Eight Winds", screenWidth / 2.f, 140.f, TA_center, 6.f);
		labels.emplace_back("Melon Jam", screenWidth / 2.f, 140.f * heightRescaling, TA_center, 6.f);

		//game verison needs to be [1]
		labels.emplace_back("GAME_VERSION", screenWidth, 39.f * heightRescaling, TA_right, 3.f);
		clickText.emplace_back("Graphics Settings", 0.f, 600.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);
		clickText.emplace_back("Audio Settings", 0.f, 680.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);

		clickText.emplace_back("Exit", 0.f, 860.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);
		clickText.emplace_back("Charmington", 0.f, 400.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);
		clickText.emplace_back("Level Creation", 0.f, 480.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);
		//clickText.emplace_back("Level Builder", 0.f, 760.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);

		Transform2dComponent transform{};
		transform.scale = { 2.f, 2.f };
		//images.emplace_back(EWETexture::addUITexture(device, "title.png"), transform);
		images.emplace_back(Texture_Builder::createSimpleTexture( "title.png", true, false, VK_SHADER_STAGE_FRAGMENT_BIT), transform);
		
	}

	void MainMenuMM::processClick(double xpos, double ypos) {
		std::pair<UIComponentTypes, int16_t> returnValues = MenuModule::checkClick(xpos, ypos);


		if (returnValues.first == UIT_ClickTextBox) {
			//printf("object comparison : %lu:%lu \n", (uint64_t)this, (uint64_t)uiPointer);


			printf("processing main menu click text box, returnValues.second : %d \n", returnValues.second);
			switch (returnValues.second) {

			case 0: {
				changeMenuState(menu_graphics_settings);
				break;
			}
			case 1: {
				changeMenuState(menu_audio_settings);
				break;
			}
			case 2: {
				clickReturns.push(MCR_ExitProgram);
				break;
			}
			case 3: {
				//printf("should not be able to enter LB mode, probably a crash \n");
				clickReturns.push(MCR_swapToCharmington);
				changeMenuState(menu_Charmington);
				break;
			}
			case 4: {
				clickReturns.push(MCR_swapToLevelCreation);
				break;
			}
			
			default: {
				//default is nothing clicked
				//return MCR_none;
			}
			}
		}
		else if (returnValues.first == UIT_TypeBox) {
			if (returnValues.second == 0) {
				//the type box now gets the callbacks on its own, given the pointers at creation
				//typeBoxes[0].giveGLFWCallbacks(windowPtr, staticMouseCallback, staticKeyCallback);
			}
		}
		else if (returnValues.first == UIT_VariableTypeBox) {
			printf("UIT_VTB - returnValues.second : %d \n", returnValues.second);
			//just give the returns off the jump, and let it handle it all the way
			//menuMap[menu_main].controlBoxes[returnValues.second].giveGLFWCallbacks(staticMouseCallback, staticKeyCallback);
		}

		//return MCR_none;
		
	}


}