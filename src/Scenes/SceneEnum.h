#pragma once

#include "MainMenuScene.h"
#include "CharmingtonScene.h"
#include "LevelCreationScene.h"

enum Scene_Enum { //need less than 256 scenes
	scene_mainmenu,
	scene_exitting,
	scene_loading,
	scene_ocean,
	scene_freeCamera, //this is specifically for capturing footage, maybe repurposed later
	scene_Charmington,
	scene_LevelCreation,

	loop_tester, //for quick testing
};