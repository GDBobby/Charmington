#pragma once
#include <EWEngine/GUI/MenuModule.h>
#include "MenuEnums.h"

namespace EWE {
	class MainMenuMM : public MenuModule {
	public:
		MainMenuMM(EWEDevice& device, float screenWidth, float screenHeight);

		void processClick(double xpos, double ypos);
	};
}