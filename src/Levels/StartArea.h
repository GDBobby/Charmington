#pragma once

#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"

namespace EWE {
	class StartLevel : public Level {
	public:
		StartLevel(EWEDevice& device) : Level{ TileSet::TS_First } {
			exits.push_back(Level_First);
			exits.push_back(Level_Second);

			TransformComponent entryTransform{};
			entryTransform.translation.x = -10.f;
			entryTransform.translation.y = 0.f;
			entryTransform.translation.z = -0.5f;

			entryTransform.rotation.y = 0.f;

			entryPoints.emplace(Level::Level_First, entryTransform);
		}
		void enterLevel(EWEDevice& device) override;
		void exitLevel() override {
			//auto materialHandler = MaterialHandler::getMaterialHandlerInstance();
			//for (auto iter = rock->ownedTextureIDs.begin(); iter != rock->ownedTextureIDs.end(); iter++) {
			//	materialHandler->removeByTransform(*iter, &rock->transform);
			//}
			rock.reset();


		}

	protected:
		std::unique_ptr<EweObject> rock;
	};
}
