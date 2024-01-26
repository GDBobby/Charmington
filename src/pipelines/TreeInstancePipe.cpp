#include "TreeInstancePipe.h"
#include <EWEngine/Graphics/Textures/Texture_Manager.h>

namespace EWE {
	TreeInstancePipe::TreeInstancePipe(EWEDevice& device) {
		//createPipeline();

		createPipeLayout(device);
		createPipeline(device);
	}

	void TreeInstancePipe::createPipeLayout(EWEDevice& device) {
		pushStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushSize = sizeof(ModelPushData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.stageFlags = pushStageFlags;
		pushConstantRange.size = pushSize;

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		std::vector<VkDescriptorSetLayout> tempDSL = {
			DescriptorHandler::getDescSetLayout(LDSL_global, device),
			TextureDSLInfo::getSimpleDSL(device, VK_SHADER_STAGE_FRAGMENT_BIT)->getDescriptorSetLayout()
		};
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(tempDSL.size());
		pipelineLayoutInfo.pSetLayouts = tempDSL.data();

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipeLayout) != VK_SUCCESS) {
			printf("failed to create background pipe layout \n");
			throw std::runtime_error("Failed to create pipe layout \n");
		}
	}
	void TreeInstancePipe::createPipeline(EWEDevice& device) {
		EWEPipeline::PipelineConfigInfo pipelineConfig{};
		EWEPipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.pipelineLayout = pipeLayout;
		//pipelineConfig.bindingDescriptions = EffectVertex::getBindingDescriptions();
		pipelineConfig.bindingDescriptions = EWEModel::getBindingDescriptions<EffectVertex>();
		pipelineConfig.attributeDescriptions = EffectVertex::getAttributeDescriptions();
		std::string vertString = "visualEffect.vert.spv";
		std::string fragString = "visualEffect.frag.spv";

		pipe = std::make_unique<EWEPipeline>(device, vertString, fragString, pipelineConfig);
	}
}