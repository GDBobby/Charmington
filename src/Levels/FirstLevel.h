#pragma once
#include <EWEngine/Graphics/EWE_Object.h>

#include "../Systems/Level.h"
#include "../Characters/Carrot.h"


namespace EWE {
	class FirstLevel : public Level {
	public:
		FirstLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device) override;
		void exitLevel() override;
		void unloadCarrot() {
			carrot.reset(nullptr);
		}
		void dropApple(EWEDevice& device, uint8_t treeID);
		void eatApple(uint8_t treeID);

		void render(FrameInfo& frameInfo) override;
		TileFlag tileAt(float x, float y) override;

		std::unique_ptr<Carrot> carrot{nullptr};
	protected:
		std::vector<EweObject> apples{};
		std::vector<EweObject> trees{};

		std::vector<std::pair<bool, glm::vec3>> treeData = {
			{false, glm::vec3{4.5f, 0.f, -3.5f}},
			{false, glm::vec3{-1.5f, 0.f, -1.f}},
			{false, glm::vec3{1.0f, 0.f, 5.5f}}
		};
	};
}
