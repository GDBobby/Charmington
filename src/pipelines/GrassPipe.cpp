#include "GrassPipe.h"

namespace EWE {
	GrassPipe::GrassPipe(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) {
		//createPipeline();

		createPipeLayout(device);
		createPipeline(device, pipeRenderInfo);
	}

	void GrassPipe::createPipeLayout(EWEDevice& device) {
		pushStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushSize = sizeof(UVScrollingPushData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.stageFlags = pushStageFlags;
		pushConstantRange.size = pushSize;

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


		std::vector<VkDescriptorSetLayout>* tempDSL = DescriptorHandler::getPipeDescSetLayout(PDSL_grass, device);
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(tempDSL->size());
		pipelineLayoutInfo.pSetLayouts = tempDSL->data();

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipeLayout) != VK_SUCCESS) {
			printf("failed to create background pipe layout \n");
			throw std::runtime_error("Failed to create pipe layout \n");
		}
	}
	void GrassPipe::createPipeline(EWEDevice& device, VkPipelineRenderingCreateInfo const& pipeRenderInfo) {
		EWEPipeline::PipelineConfigInfo pipelineConfig{};
		EWEPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.pipelineRenderingInfo = pipeRenderInfo;

		pipelineConfig.pipelineLayout = pipeLayout;
		//pipelineConfig.bindingDescriptions = EffectVertex::getBindingDescriptions();
		pipelineConfig.bindingDescriptions = GrassVertex::getBindingDescriptions();
		pipelineConfig.attributeDescriptions = GrassVertex::getAttributeDescriptions();
		std::string vertString = "grassField.vert.spv";
		std::string fragString = "grassField.frag.spv";

		pipe = std::make_unique<EWEPipeline>(device, vertString, fragString, pipelineConfig);
	}
}