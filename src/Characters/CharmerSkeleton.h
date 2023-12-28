#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

#define CHARMER_IMPORT_PATH "a"
#define CHARMER_TEXTURE_PATH "b"

namespace EWE {
	class CharmerSkeleton : public SkeletonBase {
	private:
	public:
		enum Charmer_Animations {

			Anim_idle,
		};


		CharmerSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}

