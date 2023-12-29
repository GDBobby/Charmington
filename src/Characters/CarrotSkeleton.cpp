#include "CarrotSkeleton.h"

namespace EWE {

	CarrotSkeleton::CarrotSkeleton(EWEDevice& device) : SkeletonBase{ "models/carrot", device, "carrot/" } {

		for (auto iter = fullAnimationData.begin(); iter != fullAnimationData.end(); ++iter) {

			printf("anim length? : %lu \n", iter->size());
		}
	}

	void* CarrotSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {

		return fullAnimationData[animState][animFrames].data();
	}
}