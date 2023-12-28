#pragma once
#include <EWEngine/EightWindsEngine.h>
#include <EWEngine/Scene.h>
#include "../systems/LevelManager.h"
#include "../Characters/Charmer.h"

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

	protected:
		LevelManager levelManager;
		Charmer charmer;

		bool logicActive = false;
		void logicThreadFunction();
		std::unique_ptr<std::thread> logicThread;
	};
}