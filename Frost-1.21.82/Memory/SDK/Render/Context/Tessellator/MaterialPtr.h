#pragma once

class MaterialPtr
{
public:
	/*static MaterialPtr* createMaterial(HashedString const& materialName) {
		static void* materialCreator = nullptr;

		if (materialCreator == nullptr) {
			// Sig returns 6 addresses, all of them point to the same offset
			uintptr_t sigOffset = (uintptr_t)SigManager::mce_RenderMaterialGroup_ui;
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			materialCreator = reinterpret_cast<void*>(sigOffset + offset + 7);
		}

		return Memory::CallVFunc<1, MaterialPtr*>(materialCreator, materialName);
	}*/

	static MaterialPtr* createMaterial(const HashedString& materialName) {
		/*using materialPtrConst_t = void(__fastcall*)(MaterialPtr*, __int64, const HashedString&);
		static materialPtrConst_t materialPtrConst = reinterpret_cast<materialPtrConst_t>(SigManager::mce_MaterialPtrConst);

		static __int64 renderGroupBase = 0;
		if (renderGroupBase == 0) {
			auto sig = (uintptr_t)SigManager::mce_RenderMaterialGroup_ui + 3;
			auto off = *reinterpret_cast<int*>(sig);
			renderGroupBase = sig + 4 + off;
		}

		HashedString hashedStr(materialName);
		materialPtrConst(this, renderGroupBase, hashedStr);*/

		//static auto sigOffset = getOffsetFromSig<void*>((uintptr_t)SigManager::mce_RenderMaterialGroup_ui, 3);
		//int offset = *reinterpret_cast<int*>(sigOffset + 3);

		return Memory::CallVFunc<1, MaterialPtr*, const HashedString&>(SigManager::mce_RenderMaterialGroup_ui, materialName);
	}
};