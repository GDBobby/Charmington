#pragma once
#include <EWEngine/EightWindsEngine.h>
#include <EWEngine/Scene.h>
#include "../Systems/LevelManager.h"
#include "../Characters/Charmer.h"
#include "../Characters/Carrot.h"
#include "../GUI/CharmerOverlay.h"

namespace EWE {
	class CharmingtonScene : public Scene {
		EightWindsEngine& ewEngine;
		MenuManager& menuManager;
		std::shared_ptr<SoundEngine> soundEngine;

	public:
		CharmingtonScene(EightWindsEngine& ewEngine);
		~CharmingtonScene();

		void load() override;
		void entry() override;
		void exit() override;
		bool render(double dt) override;

		void giveCharmerFocus() {
			levelManager.paused = false;
			charmer.giveInputFocus();
		}
		std::shared_ptr<CharmerOverlay> charmerOverlay;

	protected:
		Charmer charmer;
		LevelManager levelManager;


		bool logicActive = false;
		void logicThreadFunction();
		std::unique_ptr<std::thread> logicThread;
	};
}