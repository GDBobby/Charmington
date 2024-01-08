#include "TileContainer.h"


namespace EWE {
	TileContainer::TileContainer(uint16_t width, uint16_t height, EWEBuffer* indexBufferPtr, EWEBuffer* uvBufferPtr, TileSet& tileSet) : 
		width{ width }, height{ height }, size{ static_cast<uint32_t>(width) * static_cast<uint32_t>(height) },
		indexBuffer{ indexBufferPtr, sizeof(uint32_t) * 4}, 
		uvBuffer{uvBufferPtr, sizeof(glm::vec2) * 4}, 
		tileSet{tileSet},
		tileData{ reinterpret_cast<TileID*>(malloc(size * sizeof(TileID))) },
		iterData{ reinterpret_cast<TileIterator*>(malloc((((size + 1) >> 1) << 1) * sizeof(TileIterator))) }
	{

		for (uint32_t i = 0; i < size; i++) {
			tileData[i] = TILE_VOID_FLAG;
		}
	}

	TileContainer::~TileContainer() {
		free(tileData);
		free(iterData);
	}
	void TileContainer::reset() {
		memBlockCount = 0;
		instanceCount = 0;
		for (uint32_t i = 0; i < size; i++) {
			tileData[i] = TILE_VOID_FLAG;
		}
	}

	int64_t TileContainer::findMemBlock(uint32_t selectedTilePos) {
		for (uint32_t i = 0; i < memBlockCount; i++) {
			if ((selectedTilePos >= iterData[i].begin) && (selectedTilePos <= iterData[i].end)) {
				return i;
			}
		}
		return -1;
	}
	void TileContainer::removeTile(uint32_t selectedTilePos) {
		TileFindReturn foundTile = find(selectedTilePos);
		if (foundTile.memBlock == -1) {
			printf("removing a tile that doesn't exist \n");
			return;
		}
		//memory needs to be searched, it will be done in this function
		removeTileFromBuffers(foundTile.memPos);


		//memBlockCount is always going to be at least 1 greater than find location

		if (selectedTilePos == iterData[foundTile.memBlock].begin) {
			//if its the first item in a block, the block will be shrinked
			iterData[foundTile.memBlock].begin++;
			printf("shrinking the block from the front \n");
			if (iterData[foundTile.memBlock].begin > iterData[foundTile.memBlock].end) {
				//if the block is no longer valid, it is removed
				printf("the shrunk block is no longer valid, removing - %u:%u \n", memBlockCount, foundTile.memBlock);
				if (memBlockCount - foundTile.memBlock - 1) {
					memmove(&iterData[foundTile.memBlock], &iterData[foundTile.memBlock + 1], memBlockCount * sizeof(iterData));
				}
				memBlockCount--;
			}
		}
		else if (selectedTilePos == iterData[foundTile.memBlock].end) {
			//if its the final item in a block, the block will be shrinked
			printf("shrinking the block from the end \n");
			iterData[foundTile.memBlock].end--;
			//the block becoming invalid from the end should be impossible
		}
		else {
			//if its in the middle of the block, the block will be split into 2 separate blocks
			printf("tile was removed from middle, splitting the block \n");
			memmove(&iterData[foundTile.memBlock + 2], &iterData[foundTile.memBlock + 1], memBlockCount * sizeof(iterData));
			iterData[foundTile.memBlock + 1].end = iterData[foundTile.memBlock].end;
			iterData[foundTile.memBlock + 1].begin = selectedTilePos + 1;
			iterData[foundTile.memBlock].end = selectedTilePos - 1;
			memBlockCount++;
		}
	}

	void TileContainer::changeTile(uint32_t selectedTilePos, TileID changeTile) {
		tileData[selectedTilePos] = changeTile;

		//tileData[selectedTilePos] = TILE_VOID_FLAG;
		TileFindReturn foundTile = find(selectedTilePos);
		printf("changing tile, tilePos:memBlock - %u:%ld \n", selectedTilePos, foundTile.memBlock);
		if (foundTile.memBlock == -1) {
			addTile(selectedTilePos);
			return;
		}
		printf("changing tile, foundTile.memPos : %u \n", foundTile.memPos);
		tileData[selectedTilePos] = changeTile;
		changeTileUVs(selectedTilePos, foundTile.memPos);
		//updateData(selectedTilePos);
	}
	
	bool TileContainer::tryChangeTile(uint32_t selectedTilePos, TileID changeTile) {
		TileFindReturn foundTile = find(selectedTilePos);
		if (foundTile.memBlock == -1) {
			return false;
		}
		tileData[selectedTilePos] = changeTile;
		changeTileUVs(selectedTilePos, foundTile.memPos);
		return true;
		//buffers[Buffer_UV].update(tileID);

		//updateData(selectedTilePos);
	}

	void TileContainer::addTile(uint32_t selectedTilePos) {
		if (memBlockCount == 0) {
			memBlockCount++;
			iterData[0].begin = selectedTilePos;
			iterData[0].end = selectedTilePos;
			addTileToBuffers(selectedTilePos, 0);
			return;
		}
		uint32_t memPosition = 0;

		for (uint32_t memBlock = 0; memBlock < memBlockCount; memBlock++) {
			printf("searching thru the mem blocks, selectedTilePos, memBlock(begin:end) - %u - %u:%u\n", selectedTilePos, iterData[memBlock].begin, iterData[memBlock].end);
			//if its less than this current block, but not mergable with the last block
			if (selectedTilePos < iterData[memBlock].begin) {
				if (selectedTilePos == (iterData[memBlock].begin - 1)) {
					//mergable with the beginning of this block
					//checking was already done to see if this would cause merging with the last block
					printf("extending the current block from the beginning \n");
					iterData[memBlock].begin--;
					addTileToBuffers(selectedTilePos, memPosition);
					return;
				}
				//inserting a block in between this current block, and the last block
				printf("inserting a block before the current block - %u:%u \n", memBlockCount, memBlock);

				memmove(&iterData[memBlock + 1], &iterData[memBlock], (memBlockCount - memBlock) * sizeof(TileIterator));

				iterData[memBlock].begin = selectedTilePos;
				iterData[memBlock].end = selectedTilePos;
				memBlockCount++;
				printf("checking mem blocks after front insert\n");
				for (uint32_t memBlock = 0; memBlock < memBlockCount; memBlock++) {
					 printf("\t selectedTilePos, memBlock(begin:end) - % u - % u: % u\n", selectedTilePos, iterData[memBlock].begin, iterData[memBlock].end);
				}

				addTileToBuffers(selectedTilePos, memPosition);
				return;
			}
			memPosition += iterData[memBlock].size();

			if (selectedTilePos == (iterData[memBlock].end + 1)) {
				//if its mergable with the current block

				iterData[memBlock].end++;
				if ((memBlock < (memBlockCount - 1)) && (iterData[memBlock].end == (iterData[memBlock + 1].begin - 1))) {
					//if this causes mergign between this block and the next
					printf("merging this block with the next \n");
					iterData[memBlock].end = iterData[memBlock + 1].end;
					if (memBlockCount - (memBlock + 2)) {
						//if there is following blocks, they'll be moved to maintain contiguity. is contiguity a word?
						memmove(&iterData[memBlock + 1], &iterData[memBlock + 2], (memBlockCount - (memBlock + 2)) * sizeof(TileIterator));
					}
					memBlockCount--;
				}
				else {
					printf("extending the current block \n");
					
				}
				addTileToBuffers(selectedTilePos, memPosition);
				return;
			}
		}
		//if its beyond the final block, its appended to the end
		if (selectedTilePos > iterData[memBlockCount - 1].end) {
			if (selectedTilePos == (iterData[memBlockCount - 1].end + 1)) {
				//merging with the final block
				printf("merging with the final block \n");
				iterData[memBlockCount - 1].end++;
				addTileToBuffers(selectedTilePos, memPosition);
				return;
			}
			//adding a block to the end
			printf("adding a block to the end \n");

			iterData[memBlockCount].begin = selectedTilePos;
			iterData[memBlockCount].end = selectedTilePos;
			memBlockCount++;
			addTileToBuffers(selectedTilePos, memPosition);
			return;
		}

		printf("unhandled behavior in TileContainer::addTile \n");
		throw std::runtime_error("invalid case, unhandled");
		return;
	}

	void TileContainer::changeTileUVs(uint32_t tilePos, uint32_t memPosition) {

		auto uvOffsets = tileSet.getUVOffset(tileData[tilePos]);
		if (memcmp(uvOffsets.data(), (uvBuffer.memory) + (memPosition * uvBuffer.blockSize), uvBuffer.blockSize) != 0) {
			uvBuffer.copyData(memPosition, uvOffsets.data());
			uvBuffer.flush();
		}

	}

	void TileContainer::addTileToBuffers(uint32_t tilePos, uint32_t memPosition) {
		printf("adding tile to buffers, memPosition, instanceCount - %u:%u \n", memPosition, instanceCount);
		if (instanceCount > memPosition) {
			indexBuffer.shiftRight(memPosition, 1, instanceCount - memPosition);
			uvBuffer.shiftRight(memPosition, 1, instanceCount - memPosition);
		}
		//the main theme is that if its at the end, no shift is required
		addTileToBuffersAtEnd(tilePos, memPosition);
		indexBuffer.flush();
		uvBuffer.flush();

	}
	void TileContainer::addTileToBuffersAtEnd(uint32_t tilePos, uint32_t memPosition) {
		instanceCount++;

		auto indices = TileMap::getIndices(tilePos, width, height);
		indexBuffer.copyData(memPosition, indices.data());
		auto uvOffsets = tileSet.getUVOffset(tileData[tilePos]);
		uvBuffer.copyData(memPosition, uvOffsets.data());
	}

	TileFindReturn TileContainer::find(uint32_t selectedTilePos) {
		uint32_t memPos = 0;
		printf("finding tile info \n");
		for (uint32_t memBlock = 0; memBlock < memBlockCount; memBlock++) {
			printf("\tblock(begin:end) - %u - %u:%u\n", selectedTilePos, iterData[memBlock].begin, iterData[memBlock].end);
			if (selectedTilePos < iterData[memBlock].begin) {
				return { -1, memPos };
			}

			if ((selectedTilePos >= iterData[memBlock].begin) && (selectedTilePos <= iterData[memBlock].end)) {
				memPos += selectedTilePos - iterData[memBlock].begin;
				printf("returning here, %ld:%u\n", static_cast<int64_t>(memBlock), memPos);
				return { static_cast<int64_t>(memBlock), memPos };
			}
			memPos += iterData[memBlock].size();
		}
		return { -1, memPos };
		//if error checking, put a throw statement here to ensure it was found
	}

	void TileContainer::removeTileFromBuffers(uint32_t memPos) {
		//ive covered that it exists before entering this function


		indexBuffer.shiftLeft(memPos, 1, instanceCount - memPos);
		uvBuffer.shiftLeft(memPos, 1, instanceCount - memPos);

		instanceCount--;

	}
	void TileContainer::flipTile(uint32_t selectedTile, TileReadFlag flipFlag) {
		tileData[selectedTile] ^= flipFlag;
		changeTile(selectedTile, tileData[selectedTile]);

	}
	void TileContainer::interpretLoadData(uint32_t* buffer) {
		

		uint32_t currentPos = 0;
		uint32_t currentMemBlock = 0;
		uint32_t memPos = 0;
		bool inMemBlock = false;
		memcpy(tileData, buffer, size * 4);

		while (currentPos < size) {
			if ((buffer[currentPos] & TILE_VOID_FLAG) == 0) {
				iterData[currentMemBlock].begin = currentPos;
				addTileToBuffersAtEnd(currentPos, memPos);
				iterData[currentMemBlock].begin += !inMemBlock;
				iterData[currentMemBlock].end += inMemBlock;

				memPos++;
				inMemBlock = true;
			}
			else if(inMemBlock) {
				inMemBlock = false;
				currentMemBlock++;
			}
			
			currentPos++;
		}
		indexBuffer.flush();
		uvBuffer.flush();
	}
}