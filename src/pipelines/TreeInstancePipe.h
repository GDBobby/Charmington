#pragma once

//forget this, too much work. just wing it
#include <EWEngine/Systems/PipelineSystem.h>
namespace EWE {
	class TreeInstancePipe : public PipelineSystem {
	public:
		TreeInstancePipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;
	};
}
