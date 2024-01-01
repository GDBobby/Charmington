#include <EWEngine/EightWindsEngine.h>

#include "MelonJam.h"

int WinMain() {
	SettingsJSON::initializeSettings();
	EWE::EightWindsEngine ewEngine{"Melon Jam"};
	EWE::MelonJam* melonJam = nullptr;
	auto loadPart2 = [&]() {
		ewEngine.finishLoading();
		melonJam = new EWE::MelonJam(ewEngine);
		};
	std::thread loadThread(loadPart2);
	ewEngine.loadingScreen();
	loadThread.join();

	try {
		melonJam->mainThread();
	
	}
	catch (const std::exception& e) {
		std::ofstream file;
		file.open("errorLog.log");
		if (file.is_open()) {
			file << e.what() << "\n";
			file.close();
		}
		else {
			printf("try catch error \n \n");
			printf("string error : %s", e.what());
			printf("\n \n");

			//just blasting it on all channels lol
			std::cerr << e.what() << '\n';
		}
		delete melonJam;
		return EXIT_FAILURE;
	}
	delete melonJam;

	return EXIT_SUCCESS;
}