#pragma once

#include <EWEngine/systems/PipelineSystem.h>
namespace EWE {
	class BillboardPipe : public PipelineSystem {
	public:
		BillboardPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);

		void pushAndDraw(void* push) override;
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;
	};
}
