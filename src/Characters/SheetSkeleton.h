#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

namespace EWE {
	class SheetSkeleton : public SkeletonBase {
	private:
	public:
		enum Sheet_Animations {
			Anim_idle, //200
			Anim_scared, //260
			Anim_walk, //250
		};


		SheetSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}