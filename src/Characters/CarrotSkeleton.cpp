#include "CarrotSkeleton.h"

namespace EWE {

	CarrotSkeleton::CarrotSkeleton(EWEDevice& device) : SkeletonBase{ "models/carrot", device, "carrot/" } {

	}

	void* CarrotSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {

		return fullAnimationData[animState][animFrames].data();
	}
}