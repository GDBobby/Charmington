#pragma once

#include <EWEngine/Graphics/EWE_Object.h>

#include "../Systems/Level.h"
#include "../Characters/Sheet.h"
#include "../Characters/Zero.h"

namespace EWE {
	class SpookyForest : public Level {
	public:
		SpookyForest(EWEDevice& device);
		void enterLevel(EWEDevice& device) override;
		void exitLevel() override;

		void render(FrameInfo& frameInfo) override;

		bool chopTree(glm::vec2 position, glm::vec2 direction);
		TileFlag tileAt(float x, float y);
		void bark(float x, float y);

		std::unique_ptr<Sheet> sheet{ nullptr };
	protected:
		std::vector<std::pair<bool, glm::vec3>> treeData{};
		std::array<float, 4> rockCollBox = { 2.5f, -2.f, -2.f, 1.f };

		std::unique_ptr<EweObject> rock{nullptr};
		std::vector<EweObject> trees{};
	};
}
