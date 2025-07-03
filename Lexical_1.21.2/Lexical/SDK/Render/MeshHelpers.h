#pragma once
#include "../../Utils/MemoryUtil.h"
#include "ScreenContext.h"
#include "Tessellator.h"
#include "../Core/mce.h"

class MeshHelpers {
public:
	static void renderMeshImmediately(ScreenContext* screenContext, Tessellator* tessellator, mce::MaterialPtr* material) {
		using meshHelper_renderImm_t = void(__fastcall*)(ScreenContext*, Tessellator*, mce::MaterialPtr*);
		static meshHelper_renderImm_t meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(Addresses::MeshHelpers_renderMeshImmediately);
		meshHelper_renderImm(screenContext, tessellator, material);
	}
};