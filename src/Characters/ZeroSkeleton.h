#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

namespace EWE {
	class ZeroSkeleton : public SkeletonBase {
	private:
	public:
		enum Zero_Animations {
			Anim_idle,
			Anim_walk,
		};


		ZeroSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}