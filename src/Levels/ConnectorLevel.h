#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class ConnectorLevel : public Level {
	public:
		ConnectorLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;
		void render(FrameInfo& frameInfo) override;
		TileFlag tileAt(float x, float y) override;

	protected:
		void loadExtension(EWEDevice& device);

		float waterTime = 0.f;
		std::unique_ptr<EWEModel> waterModel{ nullptr };
		std::unique_ptr<EWEModel> extensionModel{ nullptr };
		TransformComponent waterTransform{};
		TransformComponent extensionTransform{};

		std::vector<TileFlag> extension{};
		uint32_t extensionWidth;
		uint32_t extensionHeight;

		TextureID extensionTexture{ TEXTURE_UNBINDED };
		std::array<TextureID, 2> waterTextures{TEXTURE_UNBINDED, TEXTURE_UNBINDED};
		//std::unique_ptr<EweObject> rock;
	};
}
