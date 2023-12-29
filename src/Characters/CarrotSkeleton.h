#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

#define CHARMER_IMPORT_PATH "a"
#define CHARMER_TEXTURE_PATH "b"

namespace EWE {
	class CarrotSkeleton : public SkeletonBase {
	private:
	public:
		enum Charmer_Animations {

			Anim_chop,
			Anim_idle,
			Anim_walk,
		};


		CarrotSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}

