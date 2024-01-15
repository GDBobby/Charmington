# Tile Container - Documentation

## Purpose
For building a level using tiles. This container is specifically used to edit a level containing an array of tiles, in a method similar to Paint programs. The end result is an array of tiles that can be loaded as a singular block at runtime, using indexes to a given tile map.

Largely, micro-optimizations aren’t required, as this container is only meant to be used outside of the gameplay loop. That aside, I would still like to optimize this as much as possible, mostly as a test of skill, and also just because I believe optimizing things on the first pass is good practice.

### Description:

The container is designed with the intention of instanced drawing using Vulkan in mind. Each tile that is meant to be drawn is in contiguous memory, regardless of it's position in the level.

Two buffers are used to achieve this. The first, tileData, tracks all active tiles, and stores them contiguously regardless of level position. The second, iterData, tracks all active tiles based on their position level.

### Side Note:
In comparison to a singular large quad, that uses one baked texture for the entire level, using an array of tiles breaks even on memory usage at about 35x35 tiles. At 1024x1024 tiles, which is somewhat large but not absurd, tile based arrays use 100x less memory. 

When the different approaches break even and when tile based pulls ahead will change based on the size of the tile map and tiles. 

I have a spreadsheet detailing the connection, make a copy if you'd like to tweak the values and see the result. 

https://docs.google.com/spreadsheets/d/1gqcAttLbkL4be-DAfsYOeU-ZaC5N719R87c1S3wfcJw/edit?usp=sharing

### Data Types
TileID - uint32_t

There is space for a maximum 268,435,455 unique tiles in a Tile Map.

The first 4 bits are reserved, according to the following enum. 

    enum TileReadFlag : uint32_t {
   	 TILE_VOID_FLAG = 0x80000000, //if a tile is not meant to be rendered, it uses this flag
   	 TILE_FLIPPED_HORIZONTALLY_FLAG = 0x40000000, 
   	 TILE_FLIPPED_VERTICALLY_FLAG = 0x20000000,
   	 TILE_FLIPPED_DIAGONALLY_FLAG = 0x10000000,
    };


## Tile Container

### Child Structures

**struct BufferInterface**
The BufferInterface struct automatically handles writing changes to the Vulkan Device. Changes to the data is controlled with the TileContainer members posData and instanceCount.

### Members:

	TileID* tileData;
	TileIterator* iterData;

	uint32_t memBlockCount;
	uint32_t instanceCount;

	uint32_t size;
	uint16_t width;
	uint16_t height;
	
	TileSet& tileSet;

**tileData:**

tileData is an array that contains all the possible tiles.

**posData:**

posData is an array that tracks the position of every active tile. Directly, this does not interact with the rendering. In combination with instance count, this is used to control changes to the buffers that are used to render the tile grid.

**memBlockCount:**

memBlockCount is used to keep track of the amount of current active memory blocks. Accessing the iterator data outside of the memory blocks will lead to undefined behavior.

**instanceCount:**

instanceCount is the amount of tiles currently active. Accessing the posData buffer at values greater than or equal to the instanceCount will lead to undefined behavior.

**size:**

size is equal to width * height, using a uint32_t to prevent overflow. I would not recommend creating a map with a width/height greater than 4096x4096. Reference the earlier spreadsheet, the size requirement at 4096x4096 is half a gigabyte, which is doubled in this particular container for tracking the positions.

**width:**

the length of each row of tiles

**height:**

length of each column of tiles

**tileSet:**

TileSet is a struct that contains the texture for the tileset. **Link to TileSet documentation.**

### Functions

    TileContainer(uint16_t width, uint16_t height, EWEBuffer* indexBufferPtr, EWEBuffer* uvBufferPtr, TileSet& tileSet);

In the constructor, the tileData buffer is initialized with all tiles being equal to TILE_VOID_FLAG. The posData array is allocated, but not initialized.

The indexBufferPtr and uvBufferPtr are used to initialized internal members of the BufferInterface struct.


    reset();

InstanceCount is set to 0, all tiles are set to TILE_VOID_FLAG.


    removeTile(uint32_t selectedTilePos);

The selected tile is removed from the active tiles. If the tile is not active, nothing happens.

    changeTile(uint32_t selectedTilePos, TileID changeTile)

If a tile is active, it is changed, by simply changing the UV values.

If the tile is not active, it is inserted into the posData buffer. following data will be shifted to make room for this new element. instanceCount is incremented.

    tryChangeTile(uint32_t selectedTilePos, TileID changeTile)

If the tile is active, it is changed and returns true.

If the tile is not active, false is returned.

    const void* getTileBuffer()

This is used to get the tileData buffer. This is explicitly recommended for saving the map to another buffer or file, and using it in any other manner will lead to undefined behavior.

    void interpretLoadData(uint32_t* buffer);

This function interprets a buffer, and is used to load maps. Each tile is read and, if necessary, added to the posData buffer.

    void bucketFill(uint32_t selectedTilePos, TileID fillTile);

This uses a Flood Fill scanline stack algorithm to replicate the Bucket tool found in many popular image editting programs.

