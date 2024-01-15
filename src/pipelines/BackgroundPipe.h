#pragma once

#include <EWEngine/Systems/PipelineSystem.h>
namespace EWE {
	class BackgroundPipe : public PipelineSystem {
	public:
		BackgroundPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo);

		EWEDescriptorSetLayout& getVertexIndexBufferLayout() {
			return *vertexIndexBufferLayout;
		}

		void drawInstanced(EWEModel* model) override;
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) override;

		std::unique_ptr<EWEDescriptorSetLayout> vertexIndexBufferLayout{ nullptr };


	};
}
