#include "SaveData.h"


SaveJSON::SaveData SaveJSON::saveData{};

void SaveJSON::initializeSettings() {
	rapidjson::Document document;

	if (!std::filesystem::exists(SAVE_LOCATION)) {
		//no file exist
		std::ofstream tempFile{ SAVE_LOCATION };
		generateDefaultFile();
		tempFile.close();
	}
	else {
		std::ifstream inFile;
		inFile.open(SAVE_LOCATION, std::ios::binary);

		// get length of file:
		inFile.seekg(0, std::ios::end);
		size_t length = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// allocate memory:
		char* buffer = new char[length + 1];

		// read data as a block:
		inFile.read(buffer, length);
		buffer[length] = '\0';

		document.Parse(buffer);
		if (document.HasParseError() || !document.IsObject()) {
			printf("error parsing settings at : %s \n", SAVE_LOCATION);
			printf("error at %d : %s \n", static_cast<int32_t>(document.GetErrorOffset()), rapidjson::GetParseError_En(document.GetParseError()));
			throw std::runtime_error("failed to parse");
			generateDefaultFile();
		}
		else {
			if (!readFromJsonFile(document)) {
				//failed to parse correctly
				printf("failed to read settings correctly \n");
				throw std::runtime_error("failed t o read settings correctly");
				generateDefaultFile();
			}
		}
		// delete temporary buffer
		delete[] buffer;

		// close filestream
		inFile.close();
	}
}

void SaveJSON::generateDefaultFile() {
	SaveData settingsDefault;
	saveData = settingsDefault;
	printf("generating default file \n");
	saveToJsonFile();
}

bool SaveJSON::readFromJsonFile(rapidjson::Document& document) {
	if (!document["version"].IsInt()) {
		printf("version not int \n");
		return false;
	}
	else if (document["version"].GetInt() != CURRENT_VERSION) {
		printf("icnorrect version \n");
		return false;
	}

	if (!document["currentMap"].IsInt()) {
		printf("currentMap not int \n");
		return false;
	}
	if (!document["currentEntrance"].IsInt()) {
		printf("SD not int \n");
		return false;
	}

	if (!document["petFlags"].IsUint64()) {
		printf("MaV not int \n");
		return false;
	}
	if (!document["obstacleFlags"].IsUint64()) {
		printf("EV not int \n");
		return false;
	}

	int valueBuffer = document["currentMap"].GetInt();
	saveData.currentMap = valueBuffer;
	valueBuffer = document["currentEntrance"].GetInt();
	saveData.currentEntrance = valueBuffer;
	uint64_t uint64Buffer = document["petFlags"].GetUint64();
	saveData.petFlags = uint64Buffer;
	uint64Buffer = document["obstacleFlags"].GetUint64();
	saveData.obstacleFlags = uint64Buffer;

	return true;
}

void SaveJSON::saveToJsonFile() {
	rapidjson::StringBuffer sb;
	rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
	writer.StartObject();
	writer.Key("version");
	writer.Int(CURRENT_VERSION);
	writer.Key("currentMap");
	writer.Int(saveData.currentMap);
	writer.Key("currentEntrance");
	writer.Int(saveData.currentEntrance);
	writer.Key("petFlags");
	writer.Uint64(saveData.petFlags);
	writer.Key("obstacleFlags");
	writer.Uint64(saveData.obstacleFlags);


	writer.EndObject();
	std::ofstream file;
	file.open(SAVE_LOCATION);
	file << sb.GetString();
	file.close();

}