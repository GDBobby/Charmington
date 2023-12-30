#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"
#include "../Characters/Sheet.h"
#include "../Characters/Zero.h"

namespace EWE {
	class SpookyForest : public Level {
	public:
		SpookyForest(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

		void render(FrameInfo& frameInfo) override;

		std::unique_ptr<Sheet> sheet;
	protected:
		std::vector<EweObject> trees;
	};
}
