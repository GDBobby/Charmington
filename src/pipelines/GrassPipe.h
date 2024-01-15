#pragma once
#include <EWEngine/Systems/PipelineSystem.h>

namespace EWE {
	class GrassPipe : public PipelineSystem {
	public:
		GrassPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;
	};
}
