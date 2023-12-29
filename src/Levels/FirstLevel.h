#pragma once
#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"
#include "../Characters/Carrot.h"


namespace EWE {
	class FirstLevel : public Level {
	public:
		FirstLevel(EWEDevice& device) : Level{TileSet::TS_First} {
			exits.push_back(Level_Start);

			TransformComponent entryTransform{};
			entryTransform.translation.x = 9.f;

			entryPoints.emplace(Level::Level_Start, entryTransform);

		}
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;
		void unloadCarrot() {
			carrot.reset(nullptr);
		}
		void dropApple(EWEDevice& device, uint8_t treeID);
		void eatApple(uint8_t treeID);

		void render(FrameInfo& frameInfo) override;

		std::unique_ptr<Carrot> carrot{nullptr};
	protected:
		std::vector<EweObject> apples;

		std::vector<std::pair<bool, glm::vec3>> treeData = {
			{false, glm::vec3{4.5f, 0.f, -3.5f}},
			{false, glm::vec3{-1.5f, 0.f, -1.f}},
			{false, glm::vec3{1.0f, 0.f, 5.5f}}
		};
	};
}
