#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

namespace EWE {
	class SheetSkeleton : public SkeletonBase {
	private:
	public:
		enum Sheet_Animations {
			Anim_idle,
			Anim_walk,
		};


		SheetSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}