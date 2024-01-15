# Tile Container - Documentation

## Purpose
For building a level using tiles. This container is specifically used to edit a level containing an array of tiles, in a method similar to Paint programs.
 The end result is an array of tiles that can be loaded as a singular block at runtime, using indexes to a given tile map.
Largely, micro-optimizations aren’t required, as this container is only meant to be used outside of the gameplay loop. That aside, I would still like to optimize this as much as possible, mostly as a test of skill, and also just because I believe optimizing things on the first pass is good practice.


### Side Note:
In comparison to a singular large quad, that uses one baked texture for the entire level, using an array of tiles breaks even on memory usage at about 35x35 tiles. At 1024x1024 tiles, which is somewhat large but not absurd, tile based arrays use 100x less memory. https://docs.google.com/spreadsheets/d/1gqcAttLbkL4be-DAfsYOeU-ZaC5N719R87c1S3wfcJw/edit?usp=sharing

### Data Types
TileID - uint32_t
	There is space for a possible 268,435,455 unique tiles in a Tile Map.
	The first 4 bits are reserved, according to the following enum. 

enum TileReadFlag : uint32_t {
   	 TILE_VOID_FLAG = 0x80000000, //if a tile is not meant to be rendered, it uses this flag
   	 TILE_FLIPPED_HORIZONTALLY_FLAG = 0x40000000, 
   	 TILE_FLIPPED_VERTICALLY_FLAG = 0x20000000,
   	 TILE_FLIPPED_DIAGONALLY_FLAG = 0x10000000,
    };


Child Structs

TileIterator:
Members -
uint32_t begin;
uint32_t end;
    struct TileIterator {
   	 uint32_t begin;
   	 uint32_t end;

   	 uint32_t size() {
   		 return (end - begin) + 1;
   	 }
    };

TileIterator is used to define memory blocks. Tiles that are not meant to be rendered, signaled by the TILE_VOID_FLAG, are outside of these memory blocks. The end is the final tile included. 
For example, if the tile at position 1 is using TileID 1, and position 0 and 2 are both using TILE_VOID_FLAG, the memory block describing this would be
{ TileIterator
{begin == 1}
{end == 1}
} 
Size returns the amount of elements inside the memory block.

    struct TileFindReturn {
   	 int64_t memBlock;
   	 uint32_t memPos;
   	 TileFindReturn() : memBlock{ -1 }, memPos{ 0 } {}
   	 TileFindReturn(int64_t memBlock, uint32_t memPos) : memBlock{ memBlock }, memPos{ memPos } {}
    };

When searching for a 
