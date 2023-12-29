#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class StartLevel : public Level {
	public:
		StartLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
		}
		void enterLevel(EWEDevice& device) override;

	protected:
		std::unique_ptr<EweObject> rock;
	};
}
