#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class StartLevel : public Level {
	public:
		StartLevel(EWEDevice& device);
		void enterLevel(EWEDevice& device, std::shared_ptr<EWEDescriptorPool> globalPool) override;
		void exitLevel() override;

	protected:
		std::unique_ptr<EweObject> rock;
	};
}
