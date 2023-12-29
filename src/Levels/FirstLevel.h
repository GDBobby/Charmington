#pragma once
#include <EWEngine/graphics/EWE_Object.h>

#include "../systems/Level.h"


namespace EWE {
	class FirstLevel : public Level {
	public:
		FirstLevel(EWEDevice& device) : Level{TileSet::TS_First} {
			exits.push_back(Level_Start);

			TransformComponent entryTransform{};
			entryTransform.translation.x = 9.f;

			entryPoints.emplace(Level::Level_Start, entryTransform);

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
