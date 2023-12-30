#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"
#include "../Characters/Sheet.h"
#include "../Characters/Zero.h"

namespace EWE {

	class ForestLevel : public Level {
	public:
		ForestLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

		void render(FrameInfo& frameInfo) override;

		TileFlag tileAt(float x, float y) override;

		bool chopTree(glm::vec2 position, glm::vec2 direction);

		std::unique_ptr<Zero> zero{nullptr};

		bool pickLog(float x, float y);
		void dropStick(float x, float y, glm::vec2 forwardDir);
		void eatStick(uint8_t ateStickID);

	protected:
		std::vector<EweObject> trees{};
		std::vector<Billboard> logs{};
		std::vector<Billboard> sticks{};
		//std::unique_ptr<EWEModel> logModel;

		std::vector<std::pair<bool, glm::vec3>> treeData{};

		TextureID logTextureID;
		TextureID stickTextureID;
	};
}
