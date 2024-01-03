#include "GridPipe.h"

namespace EWE {
	GridPipe::GridPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) {
		//createPipeline();

		createPipeLayout(device);
		createPipeline(device, pipeRenderInfo);
	}

	void GridPipe::createPipeLayout(EWEDevice& device) {
		pushStageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushSize = sizeof(Simple2DPushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.stageFlags = pushStageFlags;
		pushConstantRange.size = pushSize;

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


		std::vector<VkDescriptorSetLayout>* tempDSL = DescriptorHandler::getPipeDescSetLayout(PDSL_visualEffect, device);
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(tempDSL->size());
		pipelineLayoutInfo.pSetLayouts = tempDSL->data();

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipeLayout) != VK_SUCCESS) {
			printf("failed to create background pipe layout \n");
			throw std::runtime_error("Failed to create pipe layout \n");
		}
	}
	void GridPipe::createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) {
		EWEPipeline::PipelineConfigInfo pipelineConfig{};
		EWEPipeline::defaultPipelineConfigInfo(pipelineConfig);
		EWEPipeline::enableAlphaBlending(pipelineConfig);
		pipelineConfig.pipelineRenderingInfo = pipeRenderInfo;

		pipelineConfig.pipelineLayout = pipeLayout;
		//pipelineConfig.bindingDescriptions = EffectVertex::getBindingDescriptions();
		std::string vertString = "2dGrid.vert.spv";
		std::string fragString = "2dGrid.frag.spv";

		pipe = std::make_unique<EWEPipeline>(device, vertString, fragString, pipelineConfig);
	}
	void GridPipe::pushAndDraw(void* push) {

		vkCmdPushConstants(cmdBuf, pipeLayout, pushStageFlags, 0, pushSize, push);

		vkCmdDraw(cmdBuf, 6, 1, 0, 0);
	}
}