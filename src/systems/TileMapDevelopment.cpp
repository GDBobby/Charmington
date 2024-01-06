#include "TileMapDevelopment.h"

#include <EWEngine/GUI/UICompFunctions.h>

#include <queue>

const std::vector<uint32_t> modelIndices = {
	0,1,3,1,2,3
};

namespace EWE {
	TileMapDevelopment::TileMapDevelopment(EWEDevice& device, uint16_t width, uint16_t height) : 
		device{device},
		tileSet{ device, TileSet::TS_First }, 
		width{ width }, height{ height }, 
		modelIndexBuffer{EWEModel::createIndexBuffer(device, modelIndices)} 
	{
		init(device);
	}
	TileMapDevelopment::~TileMapDevelopment() {
		if (descriptorSet != VK_NULL_HANDLE) {
			EWEDescriptorPool::freeDescriptor(DescriptorPool_Global, &descriptorSet);
			descriptorSet = VK_NULL_HANDLE;
		}
	}
	
	void TileMapDevelopment::changeTile(uint32_t tilePosition, TileID tileID) {

		//im specifically using a vector so that the data is contiguous, as its goign to be sent to the GPU. 
		// inserting to a vector is a little unfortunate
		//probably better than whatever i would need to do to compact memory from a list or a map, not entirely sure on that though


		auto emplaceRet = tileIndexingMap.try_emplace(tilePosition, 0, tileID);
		auto uvOffsets = tileSet.getUVOffset(tileID);

		char* uvBufferPtr = reinterpret_cast<char*>(tileUVBuffer->getMappedMemory());
		char* indexBufferPtr = reinterpret_cast<char*>(tileIndexBuffer->getMappedMemory());
		const uint32_t uvBlockSize = sizeof(uvOffsets[0]) * uvOffsets.size();


		bool updatedForGPU = false;
		if (emplaceRet.second == true) {
			updatedForGPU = true;
			//instanceCount is kept track of here, and is equal to tileIndexingMap.size()
			//could do an assert to make sure equal, or just remove instanceCoutn and only use tileIndexingMap.size()

			auto indexes = TileMap::getIndices(tilePosition, width, height);
			const uint32_t indexBlockSize = sizeof(indexes[0]) * indexes.size();
			//positionIter = tileIndexingMap.emplace(tilePosition, 0).first;

			if (emplaceRet.first == tileIndexingMap.begin()) {
				//if it wasnt found, we assume this tilePosition will be the lowest active position
				emplaceRet.first->second.memoryLocation = 0;
				if (instanceCount > 0) {
					memmove(indexBufferPtr + indexBlockSize, indexBufferPtr, instanceCount * indexBlockSize);
					memmove(uvBufferPtr + uvBlockSize, uvBufferPtr, instanceCount * uvBlockSize);
				}
				instanceCount++;
				//memcpy(indexBufferPtr, indexBufferPtr, sizeof(uint32_t) * 6);
				//memcpy(uvBufferPtr, uvOffsets.data(), uvBlockSize);

				printf("after emplacing a new tilePosition at the front : %lu:%lu \n", emplaceRet.first->first, emplaceRet.first->second);
			}
			else {
				//the last positions value's are kept track of using the element on the map, and will be offset by 6 for the new tile position's offset
				printf("inserting a tile, somewhere after the beginning \n");
				instanceCount++;
				std::map<uint32_t, TileInfo>::iterator prevIter;
				for (auto iter = tileIndexingMap.begin(); iter != emplaceRet.first; iter++) {
					//finding the position of the previous active tilePosition
					printf("iterating thru the tile indexing map - map->first:tilePosition - %d:%d \n", iter->first, tilePosition);
					prevIter = iter;
				}

				printf("before this mf - %lu \n", prevIter->second);
				emplaceRet.first->second.memoryLocation = prevIter->second.memoryLocation + 1;

				if (instanceCount != emplaceRet.first->second.memoryLocation) {
					printf("moving all the following data \n");
					printf("before mem move : %lu:%lu:%lu:%lu \n", (emplaceRet.first->second.memoryLocation + 1) * indexBlockSize, emplaceRet.first->second.memoryLocation * indexBlockSize, instanceCount - emplaceRet.first->second.memoryLocation, instanceCount);
					memmove(
						indexBufferPtr + ((emplaceRet.first->second.memoryLocation + 1) * indexBlockSize),
						indexBufferPtr + (emplaceRet.first->second.memoryLocation * indexBlockSize),
						indexBlockSize * (instanceCount - emplaceRet.first->second.memoryLocation)
					);
					printf("after the memmove1 \n");
					memmove(
						uvBufferPtr + ((emplaceRet.first->second.memoryLocation + 1) * uvBlockSize),
						uvBufferPtr + (emplaceRet.first->second.memoryLocation * uvBlockSize),
						uvBlockSize * (instanceCount - emplaceRet.first->second.memoryLocation)
					);

					printf("after the memmove2 \n");
				}
				else {
					printf("placing this mf at the back \n");
				}
			}

			memcpy(
				indexBufferPtr + (emplaceRet.first->second.memoryLocation * indexBlockSize),
				indexes.data(),
				indexBlockSize
			);
			memcpy(
				uvBufferPtr + (emplaceRet.first->second.memoryLocation * uvBlockSize),
				uvOffsets.data(),
				uvBlockSize
			);
			auto iter = emplaceRet.first;
			iter++;
			for (; iter != tileIndexingMap.end(); iter++) {
				//now, all following elements will be adjusted so that theyre aware of their new position in the vector
				iter->second.memoryLocation++;
			}
		}
		else {
			//the item already exists, so we're probably just updating the UVs. 
			// a check will be performed to ensure the data has changed, since it will be uploaded to the GPU.

			
			bool foundDiff = false;
				
			if (memcmp(uvBufferPtr + (emplaceRet.first->second.memoryLocation * uvBlockSize), uvOffsets.data(), uvBlockSize) != 0) {
				foundDiff = true;
			}
			
			if (foundDiff) {
				printf("ChangeTile::updating an existing tile\n");
				updatedForGPU = true;

				memcpy(uvBufferPtr + (emplaceRet.first->second.memoryLocation * uvBlockSize), uvOffsets.data(), uvBlockSize);
			}
			else {
				printf("ChangeTile::updated but no changes found \n");
			}
		}
		if (updatedForGPU) {
			//tileUVBuffer->writeToBuffer(UVs.data(), UVs.size() * sizeof(glm::vec2));
			printf("flushing the buffer \n");
			tileIndexBuffer->flush();
			tileUVBuffer->flush();
		}

	}

	void TileMapDevelopment::removeTile(uint32_t tilePosition) {
		auto currentIter = tileIndexingMap.find(tilePosition);
		if (currentIter == tileIndexingMap.end()) {
			printf("removing tile that doesn't exist \n");
			//throw std::runtime_error("removing tile that doesn't exist");
			return;
		}

		char* uvBufferPtr = reinterpret_cast<char*>(tileUVBuffer->getMappedMemory());
		char* indexBufferPtr = reinterpret_cast<char*>(tileIndexBuffer->getMappedMemory());
		const uint32_t indexBlockSize = sizeof(uint32_t) * 4;
		const uint32_t uvBlockSize = sizeof(glm::vec2) * 4;

		uint64_t index = currentIter->second.memoryLocation;

		memmove(uvBufferPtr + (index * uvBlockSize), uvBufferPtr + (index + 1) * uvBlockSize, (instanceCount - index) * uvBlockSize);
		memmove(indexBufferPtr + (index * indexBlockSize), indexBufferPtr + (index + 1) * indexBlockSize, (instanceCount - index) * indexBlockSize);
		tileUVBuffer->flush();
		tileIndexBuffer->flush();

		auto iter = currentIter;
		iter++;
		while(iter != tileIndexingMap.end()){
			iter->second.memoryLocation--;
			iter++;
		}

		tileIndexingMap.erase(currentIter);

		instanceCount--;
	}

	void TileMapDevelopment::refreshMap(uint16_t width, uint16_t height) {

		instanceCount = 0;
		tileIndexingMap.clear();

		if ((this->width != width) || (this->height != height)) {
			tileIndexBuffer.reset();
			tileUVBuffer.reset();
			tileVertexBuffer.reset();
			this->width = width;
			this->height = height;
			constructVertices(device, tileSet.tileScale);
			constructUVsAndIndices(device);
			constructDescriptor();
			refreshedMap = true;
		}


	}
	void TileMapDevelopment::init(EWEDevice& device) {
		//createTileVertices(vertices, width, height, tileSet.tileScale);
		instanceCount = 0;
		constructVertices(device, tileSet.tileScale);
		constructUVsAndIndices(device);
		constructDescriptor();
	}

	void TileMapDevelopment::constructUVsAndIndices(EWEDevice& device) {


		tileIndexBuffer = std::make_unique<EWEBuffer>(device, sizeof(uint32_t) * 4 * width * height, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		tileUVBuffer = std::make_unique<EWEBuffer>(device, sizeof(glm::vec2) * 4 * width * height, 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		tileIndexBuffer->map();
		tileUVBuffer->map();

		//idk if i need these 2
		//memset(tileIndexBuffer->getMappedMemory(), 0, sizeof(uint32_t) * 6 * width * height);
		//memset(tileUVBuffer->getMappedMemory(), 0, sizeof(uint32_t) * 6 * width * height);

		//tileIndexBuffer->flush();
		//tileUVBuffer->flush();
	}

	void TileMapDevelopment::constructVertices(EWEDevice& device, float tileScale) {
		tileVertexBuffer = std::make_unique<EWEBuffer>(device, sizeof(glm::vec4) * (width + 1) * (height + 1), 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		tileVertexBuffer->map();
		glm::vec4* vertexMemory = reinterpret_cast<glm::vec4*>(tileVertexBuffer->getMappedMemory());

		glm::vec3 baseTranslation{ -static_cast<float>(width) / 2.f * tileScale, -static_cast<float>(height) / 2.f * tileScale, 0.f };
		//printf("base translation - %.5f:%.5f:%.5f\n", baseTranslation.x, baseTranslation.y, baseTranslation.z);
		glm::vec2 offsetPerTile{ tileScale };

		glm::vec4 vertexTrans{ baseTranslation, 1.f };
		printf("base translation ? : %.5f:%.5f \n", baseTranslation.x, baseTranslation.y);
		//vertexTrans.y -= offsetPerTile.y * 1.5f;
		//vertices right here
		for (uint32_t y = 0; y < height + 1; y++) {
			vertexTrans.x = baseTranslation.x;
			for (uint32_t x = 0; x < width + 1; x++) {
				//buildTileSquare(tileBuffer, transform, mat4, tileUVOffset[x + y * width]);
				//if ((x == 0) && (y == 0)) {
				printf("vertexTrans %d,%d x:y - %.5f:%.5f \n", x, y, vertexTrans.x, vertexTrans.y);
				//}
				*vertexMemory = vertexTrans;
				vertexMemory++;
				vertexTrans.x += offsetPerTile.x;
			}
			vertexTrans.y += offsetPerTile.y;
		}
		printf("AFTER vertexTrans %d,%d x:y - %.5f:%.5f \n", width + 1, height + 1, vertexTrans.x, vertexTrans.y);
		tileVertexBuffer->flush();

	}

	void TileMapDevelopment::renderTiles(VkCommandBuffer cmdBuf, uint8_t frameIndex) {
		if (instanceCount == 0) {
			return;
		}

		auto pipe = PipelineSystem::at(Pipe_background);

		pipe->bindPipeline();
		//pipe->bindModel(tileSet.tileModel.get());

		pipe->bindDescriptor(0, DescriptorHandler::getDescSet(DS_global, frameIndex));
		pipe->bindDescriptor(1, &descriptorSet);
		pipe->bindDescriptor(2, EWETexture::getDescriptorSets(tileSet.tileSetTexture, frameIndex));

		//ModelPushData push;
		//push.modelMatrix = floorTransform.mat4();
		//push.normalMatrix = floorTransform.normalMatrix();

		pipe->push(&pushTile);

		//pipe->drawInstanced(tileSet.tileModel.get());
		vkCmdBindIndexBuffer(cmdBuf, modelIndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(cmdBuf, 6, instanceCount, 0, 0, 0);
	}

	std::array<float, 6> const& TileMapDevelopment::updateScreenCoordinates(float screenWidth, float screenHeight) {
		//ret == left, top, right, bottom, tileWidth, tileHeight

		//in world space
		std::array<float, 4> wsCoords{
			(-(static_cast<float>(width) / 2.f * tileSet.tileScale) + pushTile.translation.x) * pushTile.scale.x * 2.f,//left
			(-(static_cast<float>(height) / 2.f * tileSet.tileScale) + pushTile.translation.y) * pushTile.scale.y * 2.f, //top
			((static_cast<float>(width) / 2.f * tileSet.tileScale) + pushTile.translation.x) * pushTile.scale.x * 2.f,//right
			((static_cast<float>(height) / 2.f * tileSet.tileScale) + pushTile.translation.y) * pushTile.scale.y * 2.f,//bottom
		};

		screenCoordinates[0] = (wsCoords[0] + 2.f) * screenWidth / 4.f;
		screenCoordinates[2] = (wsCoords[2] + 2.f) * screenWidth / 4.f;
		screenCoordinates[1] = (wsCoords[1] + 2.f) * screenHeight / 4.f;
		screenCoordinates[3] = (wsCoords[3] + 2.f) * screenHeight / 4.f;
		
		screenCoordinates[4] = tileSet.tileScale * pushTile.scale.x * screenWidth / 2.f;

		screenCoordinates[5] = tileSet.tileScale * pushTile.scale.y * screenHeight / 2.f;

		printf("screenCoordinates updated : \n\t");
		for (int i = 0; i < screenCoordinates.size(); i++) {
			printf("(%.2f)", screenCoordinates[i]);
		}
		printf("\n");
		return screenCoordinates;
	}

	void TileMapDevelopment::fitToScreen() {
		pushTile.scale.x = 2.f / (width * tileSet.tileScale);
		pushTile.scale.y = 2.f / (height * tileSet.tileScale);
		pushTile.translation.x = 0.f;
		pushTile.translation.y = 0.f;
		//at 1x1 i want scale.x = 2.f;
		//at 2x2 i want scale.x = 1.f;
		//at 3x3 i want scale.x = 0.66f;


		printf("fitting to screen : %.5f:%.5f \n", pushTile.scale.x, pushTile.scale.y);
		refreshedMap = true;
	}

	int64_t TileMapDevelopment::getClickedTile(float clickX, float clickY, float screenWidth, float screenHeight) {
		auto const& screenCoords = getScreenCoordinates(screenWidth, screenHeight);

		if ((clickX < screenCoords[0]) || (clickX > screenCoords[2]) || (clickY < screenCoords[1]) || (clickY > screenCoords[3])) {
			return -1;
		}

		int tileClickX = static_cast<int>(std::floor((clickX - screenCoords[0]) / screenCoords[4]));
		int tileClickY = static_cast<int>(std::floor((clickY - screenCoords[1]) / screenCoords[5]));

		printf(" get clicked tile : returned, max - %lu:%u \n", tileClickX + tileClickY * width, width * height);

		return tileClickX + tileClickY * width;
	}
	void TileMapDevelopment::constructDescriptor() {
		if (descriptorSet != VK_NULL_HANDLE) {
			EWEDescriptorPool::freeDescriptor(DescriptorPool_Global, &descriptorSet);
			descriptorSet = VK_NULL_HANDLE;
		}

		if (!
			EWEDescriptorWriter(((BackgroundPipe*)PipelineSystem::at(Pipe_background))->getVertexIndexBufferLayout(), DescriptorPool_Global)
			.writeBuffer(0, tileVertexBuffer->descriptorInfo())
			.writeBuffer(1, tileIndexBuffer->descriptorInfo())
			.writeBuffer(2, tileUVBuffer->descriptorInfo())
			.build(descriptorSet)
			) {
			printf("tile desc failure \n");
			throw std::runtime_error("failed to build tile map descriptor set");
		}
	}

	bool TileMapDevelopment::saveMap(std::string saveLocation) {
		if ((width == 0) || (height == 0)) {
			printf("attemptign to save with a width or height of 0\n");
			return false;
		}

		std::ofstream saveFile{saveLocation, std::ios::binary};
		if (!saveFile.is_open()) {
			printf("failed to open save file : %s \n", saveLocation.c_str());
			saveFile.open(saveLocation, std::ios::binary | std::ofstream::trunc);
			if (!saveFile.is_open()) {
				printf("failed to create file : %s \n", saveLocation.c_str());
				return false;
			}
		}
		uint64_t fileSize = width;
		fileSize *= height;
		fileSize *= 4;
		fileSize += 4;
		saveFile.width(fileSize);
		saveFile.seekp(0, std::ios::beg);

		saveFile.write(reinterpret_cast<char*>(&width), 2);
		saveFile.write(reinterpret_cast<char*>(&height), 2);

		printf("saving map with width:height - %d:%d \n", width, height);

		if (tileIndexingMap.size() == 0) {
			saveFile.fill(0);
			//printf("empty file, filling with 0s \n");
			saveFile.close();
			//maybe just return here and dont save, seems pointless?
			return false;
		}
		uint32_t indexToFillTo;

		uint32_t tilePosition = 0;

		uint32_t blankFill = 0;
		TileID fillBuffer;

		uint32_t endTilePosition = width * height;

		for (auto iter = tileIndexingMap.begin(); iter != tileIndexingMap.end(); iter++) {
			indexToFillTo = iter->first;
			while (tilePosition < indexToFillTo) {
				saveFile.write(reinterpret_cast<const char*>(&blankFill), 4);
				//printf("writing a blank at : %lu \n", saveFile.tellp());
				//saveFile.seekp(4, std::ios::cur);
				tilePosition++;
			}
			fillBuffer = iter->second.tileID + 1;
			saveFile.write(reinterpret_cast<const char*>(&fillBuffer), 4);
			//printf("writing a tile %d at : %lu \n", fillBuffer, saveFile.tellp());
			//saveFile.seekp(4, std::ios::cur);
			tilePosition = iter->first + 1;
		}
		while (tilePosition < endTilePosition) {
			saveFile.write(reinterpret_cast<const char*>(&blankFill), 4);
			//printf("writing a blank at : %lu \n", saveFile.tellp());
			//saveFile.seekp(4, std::ios::cur);
			tilePosition++;
		}

		uint64_t filePosition = saveFile.tellp();
		//printf("after saving file, instanceCount:filePosition:expectedSize - %lu:%lu:%lu", instanceCount, filePosition, fileSize);

		saveFile.close();
	}

	bool TileMapDevelopment::loadMap(std::string loadLocation) {
		std::ifstream loadFile{ loadLocation, std::ios::binary };
		if (!loadFile.is_open()) {
			printf("failed to open load file : %s \n", loadLocation.c_str());
			loadFile.open(loadLocation, std::ios::binary);
			if (!loadFile.is_open()) {
				printf("failed to create file : %s \n", loadLocation.c_str());
				return false;
			}
		}
		vkQueueWaitIdle(device.graphicsQueue());

		//printf("loading map \n");
		loadFile.seekg(0, std::ios::beg);
		uint16_t sizeBuffer;
		loadFile.read(reinterpret_cast<char*>(&sizeBuffer), 2);
		//loadFile.seekg(2, std::ios::cur);
		if (sizeBuffer != width) {
			if (sizeBuffer == 0) {
				printf("loading map with width 0? \n");
				return false;
			}
			width = sizeBuffer;
			loadFile.read(reinterpret_cast<char*>(&sizeBuffer), 2);
			//loadFile.seekg(2, std::ios::cur);
			height = sizeBuffer;

			//printf("before deconstructing bfufers, width : %d \n", width);
			tileVertexBuffer.reset();
			constructVertices(device, tileSet.tileScale);
			tileIndexBuffer.reset();
			tileUVBuffer.reset();
			constructUVsAndIndices(device);
			//printf("after destructing the buffers \n");
		}
		else {
			loadFile.read(reinterpret_cast<char*>(&sizeBuffer), 2);
			//loadFile.seekg(2, std::ios::cur);
			if (sizeBuffer != height) {
				if (sizeBuffer == 0) {
					printf("loading map with height 0?? \n");
					return false;
				}
				height = sizeBuffer;

				//printf("before deconstructing bfufers, height : %d \n", height);
				tileVertexBuffer.reset();
				constructVertices(device, tileSet.tileScale);
				tileIndexBuffer.reset();
				tileUVBuffer.reset();
				constructUVsAndIndices(device);
				//printf("after destructing the buffers \n");
			}
		}
		//printf("width and height of loaded map : %d:%d \n", width, height);

		char* indexBufferPtr = reinterpret_cast<char*>(tileIndexBuffer->getMappedMemory());
		char* uvBufferPtr = reinterpret_cast<char*>(tileUVBuffer->getMappedMemory());
		uint32_t indexBlockSize = sizeof(uint32_t) * 4;
		uint32_t uvBlockSize = sizeof(glm::vec2) * 4;
		std::array<uint32_t, 4> indexes;
		std::array<glm::vec2, 4> uvOffsets;


		uint32_t tileBuffer;
		uint32_t tilePosition = 0;
		instanceCount = 0;
		tileIndexingMap.clear();

		for (uint16_t y = 0; y < height; y++) {
			for (uint16_t x = 0; x < width; x++) {
				loadFile.read(reinterpret_cast<char*>(&tileBuffer), 4);
				//loadFile.seekg(4, std::ios::cur);
				if (tileBuffer > 0) {
					tileBuffer--;
					indexes = TileMap::getIndices(tilePosition, width, height);
					uvOffsets = tileSet.getUVOffset(tileBuffer);

					tileIndexingMap.try_emplace(tilePosition, instanceCount, tileBuffer);
					memcpy(
						indexBufferPtr + (instanceCount * indexBlockSize),
						indexes.data(),
						indexBlockSize
					);
					memcpy(
						uvBufferPtr + (instanceCount * uvBlockSize),
						uvOffsets.data(),
						uvBlockSize
					);



					instanceCount++;
				}
				tilePosition++;
			}
		}

		uint64_t filePosition = loadFile.tellg();
		//printf("after loading, instanceCount:filePosition - %lu:%lu \n", instanceCount, filePosition);
		loadFile.close();
		printf("after successfully loading fiel : %s \n", loadLocation);

		return true;
	}

	void TileMapDevelopment::selectNeighbor(uint32_t selPos, int64_t selID, std::queue<int64_t>& selection) {

	}

	void TileMapDevelopment::colorSelection(uint32_t selectPosition) {
		int64_t selectedTile = -1;

		{
			auto selectedPosIter = tileIndexingMap.find(selectPosition);
			if (selectedPosIter != tileIndexingMap.end()) {
				selectedTile = selectedPosIter->second.tileID;
			}
			//scope to deconstruct selectedPosIter here

		}

		std::queue<int64_t> selection{selectedTile};
	}
}