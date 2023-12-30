#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

namespace EWE {
	class CharmerSkeleton : public SkeletonBase {
	private:
	public:
		enum Charmer_Animations {

			Anim_idle, //80
			Anim_walk, // 160
		};


		CharmerSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}

