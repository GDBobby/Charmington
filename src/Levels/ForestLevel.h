#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class ForestLevel : public Level {
	public:
		ForestLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

	protected:
		std::vector<EweObject> trees;
	};
}
