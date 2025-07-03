#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../Render/MatrixStack.h"
#include "HashedString.h"

namespace mce {
	struct Color {
		union {
			struct {
				float r, g, b, a;
			};
			float arr[4];
		};

		Color(float red = 1.f, float green = 1.f, float blue = 1.f, float alpha = 1.f) {
			this->r = red;
			this->g = green;
			this->b = blue;
			this->a = alpha;
		}
	};

	struct Camera {
		CLASS_MEMBER(MatrixStack, matrixStack, 0x40);
	};

	struct MaterialPtr {
		static MaterialPtr* createMaterial(HashedString const& materialName) {
			static void* materialCreator = (void*)MemoryUtil::getVtableFromSig("48 8B 05 ? ? ? ? 48 8D 55 ? 48 8D 0D ? ? ? ? 48 8B 40 ? FF 15 ? ? ? ? 48 8B F8");
			return MemoryUtil::CallVFunc<1, MaterialPtr*, HashedString const&>(materialCreator, materialName);
		}
	};
}
