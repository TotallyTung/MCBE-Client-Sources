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

    struct UUID {
        uint64_t mLow;
        uint64_t mHigh;

        static UUID generate() {
            UUID uuid = {};
            uuid.mLow = (static_cast<uint64_t>(rand()) << 32) | rand();
            uuid.mHigh = (static_cast<uint64_t>(rand()) << 32) | rand();
            return uuid;
        }

        [[nodiscard]] std::string toString() const {
            // UUID bileþenlerini çýkar
            uint32_t timeLow = (mLow & 0xFFFFFFFF);
            uint16_t timeMid = (mLow >> 32) & 0xFFFF;
            uint16_t timeHiAndVersion = ((mLow >> 48) & 0x0FFF) | (4 << 12); // Version 4
            uint16_t clkSeq = (mHigh & 0x3FFF) | 0x8000; // Variant 2
            uint16_t nodeHi = (mHigh >> 16) & 0xFFFF;
            uint32_t nodeLow = (mHigh >> 32);

            // String yerine snprintf ile doðrudan hex formatlama yap
            char buffer[37]; // UUID formatý: 8-4-4-4-12 = 36 karakter + null terminator
            snprintf(buffer, sizeof(buffer),
                "%08x-%04x-%04x-%04x-%04x%08x",
                timeLow, timeMid, timeHiAndVersion, clkSeq, nodeHi, nodeLow);

            return std::string(buffer);
        }

        [[nodiscard]] bool operator==(const UUID& other) const {
            return mLow == other.mLow && mHigh == other.mHigh;
        }

        [[nodiscard]] bool operator!=(const UUID& other) const {
            return !(*this == other);
        }

        [[nodiscard]] bool operator<(const UUID& other) const {
            return mLow < other.mLow || (mLow == other.mLow && mHigh < other.mHigh);
        }

        [[nodiscard]] bool operator>(const UUID& other) const {
            return mLow > other.mLow || (mLow == other.mLow && mHigh > other.mHigh);
        }

        [[nodiscard]] bool operator<=(const UUID& other) const {
            return mLow < other.mLow || (mLow == other.mLow && mHigh <= other.mHigh);
        }

        [[nodiscard]] bool operator>=(const UUID& other) const {
            return mLow > other.mLow || (mLow == other.mLow && mHigh >= other.mHigh);
        }

        [[nodiscard]] bool operator!() const {
            return mLow == 0 && mHigh == 0;
        }

        [[nodiscard]] explicit operator bool() const {
            return mLow != 0 || mHigh != 0;
        }

        static UUID fromString(const std::string& str);
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
