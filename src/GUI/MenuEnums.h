#pragma once
#include "EWEngine/GUI/MenuEnums.h"

enum MenuClickReturn : uint16_t {
	MCR_swapToCharmington = EWE::MCR_engine_size,
	MCR_swapToMainMenu,
	MCR_CharmingtonResume,
	MCR_app_size,
};
enum MenuStates : uint16_t {
	menu_Charmington = EWE::menu_engine_size,
	menu_end,

	menu_app_size,
};

enum MenuTextureEnum : uint16_t {
	//this is the drawing order
	MT_shaderBack = EWE::MT_engine_size,

	MT_app_size,
};

