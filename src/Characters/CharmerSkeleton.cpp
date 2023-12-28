#include "CharmerSkeleton.h"

namespace EWE {
	void* CharmerSkeleton::getFinalBones(uint8_t animState, uint16_t animFrames) {
		finalMatrix = defaultMatrix;

		if (animState == Anim_idle) {
			return finalMatrix.data();
		}
		if (animState == Anim_dead) {
			if (animFrames >= partialAnimationData[animState].size()) {
				for (auto iter = partialAnimationData[animState][partialAnimationData[animState].size() - 1].begin(); iter != partialAnimationData[animState][partialAnimationData[animState].size() - 1].end(); iter++) {
					finalMatrix[iter->first] = iter->second;
				}
				return finalMatrix.data();
			}
		}

		for (auto iter = partialAnimationData[animState][animFrames].begin(); iter != partialAnimationData[animState][animFrames].end(); iter++) {
			finalMatrix[iter->first] = iter->second;
		}


		swordMatrix = finalMatrix[handBone];

		return finalMatrix.data();
	}
}