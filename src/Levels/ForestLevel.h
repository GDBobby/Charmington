#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"
#include "../Characters/Sheet.h"
#include "../Characters/Zero.h"

namespace EWE {
	struct LogStruct {
		TransformComponent transform;
		TextureID logTexture = TEXTURE_UNBINDED;
		bool drawable = false;

		LogStruct() : transform{} {}
		LogStruct(TransformComponent const& transform, TextureID logTextureID) : transform{ transform }, logTexture{ logTextureID } {
		}
	};

	class ForestLevel : public Level {
	public:
		ForestLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

		void render(FrameInfo& frameInfo) override;

		TileFlag tileAt(float x, float y) override;

		void chopTree(glm::vec2 position, glm::vec2 direction);

		std::unique_ptr<Zero> zero{nullptr};

		bool pickLog(float x, float y);

	protected:
		std::vector<EweObject> trees{};
		std::vector<LogStruct> logs{};
		//std::unique_ptr<EWEModel> logModel;

		std::vector<std::pair<bool, glm::vec3>> treeData{};

		TextureID logTextureID;
	};
}
