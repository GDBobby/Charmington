#include "MenuEndMM.h"

namespace EWE {
	MenuEndMM::MenuEndMM(EWEDevice& device, float screenWidth, float screenHeight) {
		float widthRescaling = screenWidth / DEFAULT_WIDTH;
		float heightRescaling = screenHeight / DEFAULT_HEIGHT;

		//labels.emplace_back("Eight Winds", screenWidth / 2.f, 140.f, TA_center, 6.f);
		labels.emplace_back("Thanks for playing", screenWidth / 2.f, 140.f * heightRescaling, TA_center, 6.f);

		clickText.emplace_back("Return To Main Menu", screenWidth / 2.f, 600.f * heightRescaling, TA_center, 3.f, screenWidth, screenHeight);

		clickText.emplace_back("Exit", screenWidth / 2.f, 860.f * heightRescaling, TA_center, 3.f, screenWidth, screenHeight);
		//clickText.emplace_back("Level Builder", 0.f, 760.f * heightRescaling, TA_left, 3.f, screenWidth, screenHeight);

	}

	void MenuEndMM::processClick(double xpos, double ypos) {
		std::pair<UIComponentTypes, int16_t> returnValues = MenuModule::checkClick(xpos, ypos);


		if (returnValues.first == UIT_ClickTextBox) {
			//printf("object comparison : %lu:%lu \n", (uint64_t)this, (uint64_t)uiPointer);


			printf("processing main menu click text box, returnValues.second : %d \n", returnValues.second);
			switch (returnValues.second) {

			case 0: {
				changeMenuState(menu_main);
				clickReturns.push(MCR_swapToMainMenu);
				printf("Return to main pls \n");
				break;
			}
			case 1: {
				clickReturns.push(MCR_ExitProgram);
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