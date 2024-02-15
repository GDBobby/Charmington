#include "BillboardPipe.h"

#include <EWEngine/Graphics/Texture/Texture_Manager.h>


namespace EWE {
	BillboardPipe::BillboardPipe(EWEDevice& device) {
		//createPipeline();

		createPipeLayout(device);
		createPipeline(device);
	}

	void BillboardPipe::createPipeLayout(EWEDevice& device) {
		pushStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushSize = sizeof(PointLightPushConstants);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.stageFlags = pushStageFlags;
		pushConstantRange.size = pushSize;

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


		std::vector<VkDescriptorSetLayout> tempDSL{};// = DescriptorHandler::getPipeDescSetLayout(PDSL_visualEffect, device);

		tempDSL.push_back(DescriptorHandler::getDescSetLayout(LDSL_global, device));

		TextureDSLInfo dslInfo{};
		dslInfo.setStageTextureCount(VK_SHADER_STAGE_FRAGMENT_BIT, 1);
		tempDSL.push_back(dslInfo.getDescSetLayout(device)->getDescriptorSetLayout());

		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(tempDSL.size());
		pipelineLayoutInfo.pSetLayouts = tempDSL.data();

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipeLayout) != VK_SUCCESS) {
			printf("failed to create background pipe layout \n");
			throw std::runtime_error("Failed to create pipe layout \n");
		}
	}
	void BillboardPipe::createPipeline(EWEDevice& device) {
		EWEPipeline::PipelineConfigInfo pipelineConfig{};
		EWEPipeline::defaultPipelineConfigInfo(pipelineConfig);
		EWEPipeline::enableAlphaBlending(pipelineConfig);

		pipelineConfig.pipelineLayout = pipeLayout;
		//pipelineConfig.bindingDescriptions = EffectVertex::getBindingDescriptions();
		std::string vertString = "billboard.vert.spv";
		std::string fragString = "billboard.frag.spv";

		pipe = std::make_unique<EWEPipeline>(device, vertString, fragString, pipelineConfig);
	}
	void BillboardPipe::pushAndDraw(void* push) {
		vkCmdPushConstants(cmdBuf, pipeLayout, pushStageFlags, 0, pushSize, push);

		vkCmdDraw(cmdBuf, 6, 1, 0, 0);
	}
}