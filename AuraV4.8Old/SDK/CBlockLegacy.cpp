#include "CBlockLegacy.h"

#include "../Memory/GameData.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"

C_Block* C_BlockSource::getBlock(const vec3_ti& block) {
	using getBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 8B 4A"));
	return getBlock(this, block);
}
C_BlockActor* C_BlockSource::getBlockEntity(const vec3_ti& block) {
	using getBlockEntity_t = C_BlockActor*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("40 55 56 57 48 83 EC ?? 8B 02"));
	return getBlockEntity(this, block);
}
float C_BlockSource::getSeenPercent(const vec3_t& pos, const AABB& target) {
	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, const vec3_t&, const AABB&);
	static getSeenPercent_t Func = reinterpret_cast<getSeenPercent_t>(Utils::getBase() + 0x22FE020);
	return Func(this, pos, target);
}
C_Block* C_BlockSource::getLiquidBlock(const vec3_ti& block) {
	using getLiquidBlock_t = C_Block*(__fastcall*)(C_BlockSource*, const vec3_ti&);
	static getLiquidBlock_t getLiquidBlock = Utils::FuncFromSigOffset<getLiquidBlock_t>(FindSignature("E8 ?? ?? ?? ?? 65 48 8B 0C 25 ?? ?? ?? ??"), 1);
	return getLiquidBlock(this, block);
}
bool C_BlockLegacy::getCollisionShape(AABB* collShapeOut, C_Block* block, C_BlockSource* blockSource, const vec3_ti* pos, C_Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, C_Block*, C_BlockSource*, const vec3_ti*, C_Entity*>(this, collShapeOut, block, blockSource, pos, 0);
}
void C_BlockLegacy::liquidGetFlow(vec3_t* flowOut, C_BlockSource* reg, const vec3_ti* pos) {
	using liquid_getFlow_t = int(__fastcall*)(C_BlockLegacy*, vec3_t*, C_BlockSource*, const vec3_ti*);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("E8 ? ? ? ? 48 8B 4C 24 ? F3 0F 10 01"), 1);

	liquid_getDepth(this, flowOut, reg, pos);
}
bool C_BlockLegacy::hasWater(C_BlockSource* reg, const vec3_ti& pos) {
	if (this->material->isLiquid) {
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}
