#include "Level.h"

namespace EWE {
	

	void Terrain_Object::render(FrameInfo& frameInfo) {
		TextureID currentlyBinded = TEXTURE_UNBINDED;
		SimplePushConstantData push;

		model->bind(frameInfo.cmdIndexPair.first);
		for (auto& objectType : objectTypes) {
			/*
			if (objectType.first != currentlyBinded) {
				vkCmdBindDescriptorSets(frameInfo.cmdIndexPair.first,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					pipeLayout,
					1, 1,
					EWETexture::getDescriptorSets(objectType.first, frameInfo.cmdIndexPair.second),
					0, nullptr
				);
				currentlyBinded = objectType.first;
			}

			for (auto& object : objectType.second) {
				if (object.active) {
					push.modelMatrix = object.transform.mat4();
					push.normalMatrix = object.transform.normalMatrix();
					vkCmdPushConstants(frameInfo.cmdIndexPair.first, pipeLayout, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, sizeof(SimplePushConstantData), &push);
					model->draw(frameInfo.cmdIndexPair.first);
				}
			}
			*/
		}


	}


	std::unique_ptr<EWEModel> Level::floor;

	void Level::render(FrameInfo& frameInfo) {
		auto pipe = PipelineSystem::at(Pipe_background);
		//floor->bind(frameInfo.cmdIndexPair.first);
		pipe->bindPipeline();
		pipe->bindModel(floor.get());
		pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameInfo.cmdIndexPair.second));
		pipe->bindDescriptor(1, EWETexture::getDescriptorSets(floorTextureID, frameInfo.cmdIndexPair.second));

		ModelPushData push;
		push.modelMatrix = floorTransform.mat4();
		//push.normalMatrix = floorTransform.normalMatrix();
		pipe->pushAndDraw(&push);
		return;


		for (int i = 0; i < terrainObjects.size(); i++) {
			terrainObjects[i].render(frameInfo);
		}
	}
}