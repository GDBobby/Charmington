#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

namespace EWE {
	class CarrotSkeleton : public SkeletonBase {
	private:
	public:
		enum Carrot_Animations {

			Anim_chop,
			Anim_eat,
			Anim_idle,
			Anim_walk,
		};


		CarrotSkeleton(EWEDevice& device);

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}

