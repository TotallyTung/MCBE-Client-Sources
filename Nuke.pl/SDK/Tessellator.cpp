#include "Tessellator.h"

mce::Mesh* BlockTessellator::getMeshForBlockInWorld(Tessellator *t, Block *block, Vec3i &pos) {
	using BT_getMeshForBlockInWorld_t = mce::Mesh* (*)(BlockTessellator *, Tessellator *, Block *, Vec3i &, int, __int64);
	static BT_getMeshForBlockInWorld_t getMeshForBlock = reinterpret_cast<BT_getMeshForBlockInWorld_t>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4D 8B F1 4D 8B F8 48"));

	return getMeshForBlock(this, t, block, pos, block->getRenderLayer(), 0);
}

void Tessellator::color(float r, float g, float b, float a) {
	using tess_color_t = void(__fastcall*)(Tessellator*, float, float, float, float);
	static tess_color_t tess_color = reinterpret_cast<tess_color_t>(FindSignature("80 b9 ? ? ? ? ? 4c 8b c1 75"));
	tess_color(this, r, g, b, a);
}