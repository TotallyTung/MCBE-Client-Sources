#pragma once
#include "../../Utils/MemoryUtils.h"
#include "HashedString.h"

class MaterialPtr {
public:
	static MaterialPtr* createMaterial(HashedString const& materialName) {
		static void* materialCreator = nullptr;

		if (materialCreator == nullptr) {
			// Sig returns 6 addresses, all of them point to the same offset
			uintptr_t sigOffset = findSig(Sigs::materialptr::createMaterial);
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			materialCreator = reinterpret_cast<void*>(sigOffset + offset + 7);
		}

		return MemoryUtils::CallVFunc<1, MaterialPtr*>(materialCreator, materialName);
	}
};
