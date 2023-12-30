#pragma once
#include <EWEngine/GUI/MenuModule.h>
#include "MenuEnums.h"

namespace EWE {
	class GameMenuMM : public MenuModule {
	public:
		GameMenuMM(EWEDevice& device, float screenWidth, float screenHeight);

		void processClick(double xpos, double ypos);
	};
}