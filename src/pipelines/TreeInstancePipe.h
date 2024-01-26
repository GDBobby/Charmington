#pragma once

//forget this, too much work. just wing it
#include <EWEngine/Systems/PipelineSystem.h>
namespace EWE {
	class TreeInstancePipe : public PipelineSystem {
	public:
		TreeInstancePipe(EWEDevice& device);
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device) override;
	};
}
