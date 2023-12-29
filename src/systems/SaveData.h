#pragma once

//#include "Settings.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/error/en.h>


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>


typedef uint16_t LevelID;


#define CURRENT_VERSION 10
#define SAVE_LOCATION "save.json"

class SaveJSON {
public:
	enum PetFlags : uint64_t {
		PF_Carrot = 1,
		PF_Cat = 1 << 1,
		PF_Dog = 1 << 2,
	};

	struct SaveData {
		int version = 10;
		int currentMap = 0;
		int currentEntrance = 0;

		uint64_t petFlags = 0;
		uint64_t obstacleFlags = 0;
	};
	static SaveData saveData;

	static void initializeSettings();

	static void generateDefaultFile();
	static bool readFromJsonFile(rapidjson::Document& document);
	static void saveToJsonFile();
};