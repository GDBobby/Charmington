#pragma once

#include <EWEngine/SkeletonHandlerBase.h>

#define CHARMER_IMPORT_PATH "a"
#define CHARMER_TEXTURE_PATH "b"

namespace EWE {
	class CharmerSkeleton : public SkeletonBase {
	private:
	public:
		enum Charmer_Animations {
			Anim_attack,
			Anim_dead,
			Anim_stun,
			Anim_walking,


			Anim_idle,
		};


		CharmerSkeleton(EWEDevice& device) : SkeletonBase{ CHARMER_IMPORT_PATH, device, CHARMER_TEXTURE_PATH } {

		}

		void* getFinalBones(uint8_t animState, uint16_t animFrames) override;
	};
}

