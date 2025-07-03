#pragma once

class BlockPalette {
public:
	class Block* getBlock(uint32_t const& runtimeId) {
		return Memory::CallFunc<Block*>(SigManager::BlockPalette_getBlock, runtimeId, this);
	}
};