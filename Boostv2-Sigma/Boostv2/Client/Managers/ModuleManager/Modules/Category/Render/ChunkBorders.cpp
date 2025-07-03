#include "ChunkBorders.h"

ChunkBorders::ChunkBorders() : Module("ChunkBorders", "Show chunk borders", Category::RENDER) {
}

void ChunkBorders::onDisable() {
	if (showChunkBorderBoolPtr != nullptr) {
		*showChunkBorderBoolPtr = false;
	}
}

void ChunkBorders::onClientTick() {
	if (showChunkBorderBoolPtr != nullptr) {
		*showChunkBorderBoolPtr = true;
	}
}