#pragma once

#include "TileSet.h"
#include "TileMap.h"

//typedef uint32_t TileID;

namespace EWE {
	struct TileNode {
		TileID tileID;
	};
	struct TileIterator {
		uint32_t begin;
		uint32_t end;

		uint32_t size() {
			return (end - begin) + 1;
		}
	};
	struct TileFindReturn {
		int64_t memBlock;
		uint32_t memPos;
		TileFindReturn() : memBlock{ -1 }, memPos{ 0 } {}
		TileFindReturn(int64_t memBlock, uint32_t memPos) : memBlock{ memBlock }, memPos{ memPos } {}
	};


	class TileContainer {
	public:
		struct BufferInterface {
			EWEBuffer* bufferPtr;
			char* memory;
			uint64_t blockSize;
			BufferInterface(EWEBuffer* bufferPtr, uint64_t blockSize) : bufferPtr{ bufferPtr }, memory{ reinterpret_cast<char*>(bufferPtr->getMappedMemory()) }, blockSize{blockSize} {}
			
			void flush() {
				bufferPtr->flush();
			}/*
			void flushMin(uint32_t memIndex) {
				bufferPtr->flushMin(memIndex * blockSize);
			}*/

			void shiftRight(uint32_t memoryIndex, uint32_t shiftAmount, uint32_t shiftSize) {
				memmove(memory + (memoryIndex + shiftAmount) * blockSize, memory + (memoryIndex)*blockSize, shiftSize * blockSize);
			}
			void shiftLeft(uint32_t memoryIndex, uint32_t shiftAmount, uint32_t shiftSize) {
				memmove(memory + (memoryIndex)*blockSize, memory + (memoryIndex + shiftAmount) * blockSize, shiftSize * blockSize);
			}
			void copyData(uint32_t memoryIndex, void* data) {
				memcpy(memory + (memoryIndex * blockSize), data, blockSize);
			}
		};


		enum Buffer_Enum : uint8_t {
			Buffer_Index,
			Buffer_UV,
			Buffer_Size,
		};
	protected:
		BufferInterface indexBuffer;
		BufferInterface uvBuffer;

		uint32_t memBlockCount{ 0 }; //this is the count of data blocks
		uint32_t instanceCount{ 0 };
		uint32_t size;
		uint16_t width;
		uint16_t height;

		TileID* tileData;
		TileIterator* iterData;

		TileSet& tileSet;

	public:
		TileContainer(uint16_t width, uint16_t height, EWEBuffer* indexBufferPtr, EWEBuffer* uvBufferPtr, TileSet& tileSet);
		~TileContainer();
		uint32_t getMemoryBlockCount() {
			return memBlockCount;
		}
		uint32_t getSize() {
			return size;
		}
		uint32_t getInstanceCount() {
			return instanceCount;
		}
		void* getTileBuffer() {
			return tileData;
		}

		//returns data block index
		TileFindReturn find(uint32_t selectedTilePos);
		void removeTile(uint32_t selectedTilePos);
		void changeTile(uint32_t selectedTilePos, TileID changeTile);
		bool tryChangeTile(uint32_t selectedTilePos, TileID changeTile);

		void reset();
		void flipTile(uint32_t selectedTile, TileReadFlag flipFlag);

		void interpretLoadData(uint32_t* buffer);

	protected:
		//internal use only
		//checking should be done before entering to ensure the block being added does not currently exist
		void addTile(uint32_t selectedTilePos);

		//buffer functions
		void addTileToBuffers(uint32_t tilePos, uint32_t memPosition);
		void addTileToBuffersAtEnd(uint32_t tilePos, uint32_t memPosition);
		void changeTileUVs(uint32_t tilePos, uint32_t memPosition);
		void removeTileFromBuffers(uint32_t memPos);
		int64_t findMemBlock(uint32_t selectedTilePos);
	};
}