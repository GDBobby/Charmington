#pragma once
#include <EWEngine/EightWindsEngine.h>
#include <EWEngine/Scene.h>
#include "../systems/LevelManager.h"
#include "../Characters/Charmer.h"
#include "../Characters/Carrot.h"

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
			charmer.giveInputFocus();
		}

	protected:
		Charmer charmer;
		LevelManager levelManager;

		bool logicActive = false;
		void logicThreadFunction();
		std::unique_ptr<std::thread> logicThread;
	};
}