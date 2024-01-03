#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class StartLevel : public Level {
	public:
		StartLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device) override;
		void exitLevel() override;

		bool chopTree(glm::vec2 position, glm::vec2 direction);

	protected:
		std::vector<EweObject> stumps{};
		std::vector<glm::vec2> stumpPositions = {
			glm::vec2{1.f, -11.f},
			glm::vec2{-1.f, -11.f},
		};
	};
}
