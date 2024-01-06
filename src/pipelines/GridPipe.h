#pragma once

#include <EWEngine/systems/PipelineSystem.h>
namespace EWE {
	class GridPipe : public PipelineSystem {
	public:
		GridPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);

		//void pushAndDraw(void* push) override;
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;
	};
}
