#include "GridPipe.h"

#include <EWEngine/Graphics/Textures/Texture_Manager.h>

namespace EWE {
	GridPipe::GridPipe(EWEDevice& device) {
		//createPipeline();

		createPipeLayout(device);
		createPipeline(device);
	}

	void GridPipe::createPipeLayout(EWEDevice& device) {
		pushStageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushSize = sizeof(Grid2DPushConstantData);

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
	void GridPipe::createPipeline(EWEDevice& device) {
		EWEPipeline::PipelineConfigInfo pipelineConfig{};
		EWEPipeline::defaultPipelineConfigInfo(pipelineConfig);
		EWEPipeline::enableAlphaBlending(pipelineConfig);

		pipelineConfig.pipelineLayout = pipeLayout;
		pipelineConfig.bindingDescriptions = EWEModel::getBindingDescriptions<VertexGrid2D>();
		pipelineConfig.attributeDescriptions = VertexGrid2D::getAttributeDescriptions();
		std::string vertString = "2dGrid.vert.spv";
		std::string fragString = "2dGrid.frag.spv";

		pipe = std::make_unique<EWEPipeline>(device, vertString, fragString, pipelineConfig);
	}

	//void GridPipe::pushAndDraw(void* push) {

	//	vkCmdPushConstants(cmdBuf, pipeLayout, pushStageFlags, 0, pushSize, push);

	//	vkCmdDraw(cmdBuf, 6, 1, 0, 0);
	//}
}