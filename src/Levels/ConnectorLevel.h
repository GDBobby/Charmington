#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"
#include "../Characters/Sheet.h"

namespace EWE {
	class ConnectorLevel : public Level {
	public:
		ConnectorLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device) override;
		void exitLevel() override;
		void render(FrameInfo& frameInfo) override;
		TileFlag tileAt(float x, float y) override;

		bool dropPlank(float x, float y, glm::vec2 forwardDir);
		void bark(float x, float y);

		std::unique_ptr<Sheet> sheet{nullptr};
		bool scaredSheet = false;
		bool builtBridge = false;
		bool placedPlanks = false;
	protected:
		void loadExtension(EWEDevice& device);
		std::vector<EweObject> planks{};
		std::vector<glm::vec3> plankTranslations = {
			glm::vec3{2.5f, 0.f, -19.25f},
			glm::vec3{2.5f, 0.f, -19.75f},
			glm::vec3{2.5f, 0.f, -20.25f},
			glm::vec3{2.5f, 0.f, -20.75f},
			glm::vec3{2.5f, 0.f, -21.25f},
			glm::vec3{2.5f, 0.f, -21.75f},
			glm::vec3{2.5f, 0.f, -22.25f},
		};
		uint8_t placedPlankCount = 0;


		float waterTime = 0.f;
		std::unique_ptr<EWEModel> waterModel{ nullptr };
		std::unique_ptr<EWEModel> extensionModel{ nullptr };
		TransformComponent waterTransform{};
		TransformComponent extensionTransform{};
		uint8_t scareCount = 0;

		std::vector<TileFlag> extension{};
		uint32_t extensionWidth;
		uint32_t extensionHeight;

		TextureID extensionTexture{ TEXTURE_UNBINDED };
		std::array<TextureID, 2> waterTextures{TEXTURE_UNBINDED, TEXTURE_UNBINDED};
		//std::unique_ptr<EweObject> rock;
	};
}
