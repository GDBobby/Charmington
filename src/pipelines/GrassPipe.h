#pragma once
#include <EWEngine/Systems/PipelineSystem.h>

namespace EWE {
	class GrassPipe : public PipelineSystem {
	public:
		GrassPipe(EWEDevice& device);
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device) override;
	};
}
