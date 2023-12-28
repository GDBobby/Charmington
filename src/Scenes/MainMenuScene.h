#pragma once
#include <EWEngine/EightWindsEngine.h>
#include <EWEngine/Scene.h>

namespace EWE {
	class MainMenuScene : public Scene {
		EightWindsEngine& ewEngine;
		MenuManager& menuManager;
		std::shared_ptr<SoundEngine> soundEngine;

	public:
		MainMenuScene(EightWindsEngine& ewEngine);
		~MainMenuScene();

		void load() override;
		void entry() override;
		void exit() override;
		bool render(double dt) override;

	protected:
	};
}