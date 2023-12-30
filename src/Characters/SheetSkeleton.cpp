#include "SheetSkeleton.h"

namespace EWE {

	SheetSkeleton::SheetSkeleton(EWEDevice& device) : SkeletonBase{ "models/sheet", device, "sheet/" } {

		for (auto iter = fullAnimationData.begin(); iter != fullAnimationData.end(); ++iter) {

			printf("anim length? : %lu \n", iter->size());
		}
	}

	void* SheetSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {

		return fullAnimationData[animState][animFrames].data();
	}
}