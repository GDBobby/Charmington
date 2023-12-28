#pragma once

#include <EWEngine/systems/PipelineSystem.h>
namespace EWE {
	class BackgroundPipe : public PipelineSystem {
	public:
		BackgroundPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;
	};
}
