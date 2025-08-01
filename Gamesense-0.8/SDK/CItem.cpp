#include "CItem.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"
#include "Tag.h"

C_BaseActorRenderContext::C_BaseActorRenderContext(C_ScreenContext* ScreenCtx, C_ClientInstance* client, MinecraftGame* game) {
	memset(this, 0, sizeof(C_BaseActorRenderContext));
	using BaseActorRenderContext_t = __int64(__fastcall*)(C_BaseActorRenderContext*, C_ScreenContext*, C_ClientInstance*, MinecraftGame*);
	static BaseActorRenderContext_t BaseActorRenderContext_constructor = reinterpret_cast<BaseActorRenderContext_t>(FindSignature("48 89 5C 24 ?? 48 89 74 24 ?? 48 89 4C 24 08 57 48 83 EC ?? ?? ?? ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 ?? 33 F6 48 89 71 08"));
	BaseActorRenderContext_constructor(this, ScreenCtx, client, game);
}
void C_ItemRenderer::renderGuiItemNew(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, int mode, float x, float y, float opacity, float scale, bool isEnchanted) {
	using renderGuiItemNew_t = void(__fastcall*)(C_ItemRenderer*, C_BaseActorRenderContext*, C_ItemStack*, int, float, float, bool, float, float, float);
	static renderGuiItemNew_t renderGuiItemNew = reinterpret_cast<renderGuiItemNew_t>(FindSignature("48 8B C4 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 44 89 8C 24"));
	item->setShowPickUp(false);
	renderGuiItemNew(this, BaseActorRenderCtx, item, mode, x, y, isEnchanted, 1, opacity, scale);
}

void C_ItemRenderer::renderGuiItemInChunk(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, float x, float y, float opacity, float scale, bool isEnchanted, int mode) {
	using renderGuiItemInChunk_t = void(__fastcall*)(C_ItemRenderer*, C_BaseActorRenderContext*, __int64, C_ItemStack*, float, float, float, float, float, int);
	static renderGuiItemInChunk_t renderGuiItem = reinterpret_cast<renderGuiItemInChunk_t>(FindSignature("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 48 8B 05 ? ? ? ?"));
	renderGuiItem(this, BaseActorRenderCtx, 2, item, x, y, 1, 1, scale, 0);
}

C_ItemStack::C_ItemStack(const C_ItemStack& src) {
	memset(this, 0x0, sizeof(C_ItemStack));
	using ItemStackCopyConstructor_t = void(__fastcall*)(C_ItemStack&, C_ItemStack const&);
	static ItemStackCopyConstructor_t ItemStackCopyConstructor = reinterpret_cast<ItemStackCopyConstructor_t>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B F2 48 8B F1 48 89 4C 24 ? 33 ED 89 6C 24 20 48 8D"));
	ItemStackCopyConstructor(*this, src);
	this->setVtable();
}
C_ItemStack::C_ItemStack(const Tag& tag) {
	memset(this, 0x0, sizeof(C_ItemStack));
	this->setVtable();
	fromTag(tag);
}
void C_ItemStack::fromTag(const Tag& tag) {
	using ItemStackBase__loadItemF = void(__fastcall*)(C_ItemStack*, Tag const&);
	static ItemStackBase__loadItemF fromTag = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 37 48 8B DA 48 8B F9 48 89 4D 97 0F 57 C0 0F 11 45 A7 0F 11 45 B7 0F 11 45 C7 0F 11 45 D7 0F 11 45 E7 0F 11 45 F7 0F 11 45 07"));
	fromTag(this, tag);
}

void C_ItemStack::save(CompoundTag** tag) {
	using ItemStackBase__saveF = void(__fastcall*)(C_ItemStack*, CompoundTag**);
	ItemStackBase__saveF save = reinterpret_cast<ItemStackBase__saveF>(FindSignature("48 89 5C 24 ?? 55 56 57 41 56 41 57 48 8D 6C 24 C9 48 81 EC ?? ?? ?? ?? 48 8B FA 48 8B F1 48 89 55 0F 45 33 ?? 44 89 7D"));
	return save(this, tag);
}
void C_ItemStack::setUserData(std::unique_ptr<Tag> tag) {
	using setUserData_t = void(__fastcall*)(C_ItemStack*, std::unique_ptr<Tag>);
	setUserData_t setUserData = reinterpret_cast<setUserData_t>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B FA 48 8B F1 48 89 94 24"));
	setUserData(this, std::move(tag));
}
void C_ItemStack::reinit(C_BlockLegacy& legacy, int count) {
	this->setVtable();
	Utils::CallVFunc<2, void>(this, &legacy, count);
}
void C_ItemStack::reinit(C_Item& item, int count, int itemData) {
	this->setVtable();
	Utils::CallVFunc<3, void>(this, &item, count, itemData);
}
int C_ItemStack::getEnchantValue(int enchantId) {
	using getEnchantsLevel_t = int(__fastcall*)(int, C_ItemStack*);
	static getEnchantsLevel_t getEnchantsLevel = reinterpret_cast<getEnchantsLevel_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 0F B6 D9 33 FF 48 8B CA E8 ? ? ? ?"));
	return getEnchantsLevel(enchantId, this);
}
void C_ItemStack::setVtable(void) {
	static uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? C6 05 ? ? ? ? ? 48 83 C4");
	int offset = *reinterpret_cast<int*>(sigOffset + 3);
	this->vTable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
}
C_Item*** ItemRegistry::getItemFromId(void* ptr, int itemId) {
	using getItemFromId_t = C_Item * **(__fastcall*)(void*, int);
	static getItemFromId_t getItemFromId = reinterpret_cast<getItemFromId_t>(FindSignature("40 53 48 83 EC ? 8D 42 ? 48 8B D9 66 83 F8 ? 0F 86 ? ? ? ? 44 0F BF C2 49 B9 ? ? ? ? ? ? ? ? 41 8B C0"));
	return getItemFromId(ptr, itemId);
}

C_Item*** ItemRegistry::lookUpByName(void* a1, void* a2, TextHolder& text) {
	using ItemRegistry__lookupByName_t = C_Item * **(__fastcall*)(void*, void*, TextHolder&);
	static ItemRegistry__lookupByName_t ItemRegistry__lookupByNameF = reinterpret_cast<ItemRegistry__lookupByName_t>(FindSignature("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ? 4C 8B EA 48 8B F9 48 89 4D A7 49 83 78"));
	return ItemRegistry__lookupByNameF(a1, a2, text);
}

ItemDescriptor::ItemDescriptor(int id, int16_t itemData) {
	using ItemDescriptor__ItemDescriptor_t = ItemDescriptor * (__fastcall*)(ItemDescriptor*, int, int16_t);
	static ItemDescriptor__ItemDescriptor_t func = reinterpret_cast<ItemDescriptor__ItemDescriptor_t>(FindSignature("48 89 5C 24 ? 55 56 57 48 83 EC ? 33 ED 48 8B D9 48 89 29 41 8B F8 48 89 69 ? 8B F2 48 89 69 ? 66 89 69 ? C6 41"));
	func(this, id, itemData);
}
C_Item *C_Item::setAllowOffhand(bool allow) {
	using setAllowOffhand_t = C_Item*(__fastcall *)(C_Item*, bool);
	setAllowOffhand_t setAllowOffhand = reinterpret_cast<setAllowOffhand_t>(FindSignature("80 89 22 01 00 00 ? 48 8B ? C3 CC CC CC CC CC 40"));
	return setAllowOffhand(this, allow);
}