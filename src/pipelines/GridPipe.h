#pragma once

#include <EWEngine/Systems/PipelineSystem.h>
namespace EWE {
	class GridPipe : public PipelineSystem {
	public:
		GridPipe(EWEDevice& device);

		//void pushAndDraw(void* push) override;
	private:
		void createPipeLayout(EWEDevice& device) override;
		void createPipeline(EWEDevice& device) override;
	};
}
