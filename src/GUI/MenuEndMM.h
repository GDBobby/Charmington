#pragma once
#include <EWEngine/GUI/MenuModule.h>
#include "MenuEnums.h"

namespace EWE {
	class MenuEndMM : public MenuModule {
	public:
		MenuEndMM(EWEDevice& device, float screenWidth, float screenHeight);

		void processClick(double xpos, double ypos);
	};
}