#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class StartLevel : public Level {
	public:
		StartLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

		void chopTree(glm::vec2 position, glm::vec2 direction);

	protected:
		//std::unique_ptr<EweObject> rock;
		std::vector<EweObject> stumps{};
		std::vector<glm::vec2> stumpPositions = {
			glm::vec2{1.f, -11.f},
			glm::vec2{-1.f, -11.f},
		};
	};
}
