#include "CharmerSkeleton.h"

namespace EWE {

	CharmerSkeleton::CharmerSkeleton(EWEDevice& device) : SkeletonBase{ "models/charmer", device, "charmer/" } {

	}

	void* CharmerSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {

		return fullAnimationData[animState][animFrames].data();
	}
}