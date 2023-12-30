#include "ZeroSkeleton.h"

namespace EWE {

	ZeroSkeleton::ZeroSkeleton(EWEDevice& device) : SkeletonBase{ "models/zero", device, "zero/" } {

		for (auto iter = fullAnimationData.begin(); iter != fullAnimationData.end(); ++iter) {

			printf("anim length? : %lu \n", iter->size());
		}
	}

	void* ZeroSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {

		return fullAnimationData[animState][animFrames].data();
	}
}