// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/MemoryUtils.h"
#include "MaterialPtr.h"

class ScreenContext;

enum class VertextFormat {
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

class Tessellator {
private:
	char pad_0000[0x184];
	Vec3<float> transformOffset; // 0x184
public:

	inline void begin(VertextFormat format, int reservedVerticies = 0) {
		using tess_begin_t = void(__fastcall*)(Tessellator*, VertextFormat, int);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(findSig(Sigs::tessellator::begin));
		tess_begin(this, format, reservedVerticies);
	}

	inline void vertex(float x, float y, float z) {
		using tess_vertex_t = void(__fastcall*)(Tessellator*, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(findSig(Sigs::tessellator::vertex));
		tess_vertex(this, x, y, z);
	}

	inline void color(float r, float g, float b, float a) {
		using tess_color_t = void(__fastcall*)(Tessellator*, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(findSig(Sigs::tessellator::color));
		tess_color(this, r, g, b, a);
	}

	inline void setRotation(float angle, Vec3<float> pivot) {
		using tess_setRotation_t = void(__fastcall*)(Tessellator*, float, Vec3<float>);
		static tess_setRotation_t tess_setRotation = reinterpret_cast<tess_setRotation_t>(findSig(Sigs::tessellator::setRotation));
		tess_setRotation(this, angle, pivot);
	}

	inline void resetTransform(bool a1) {
		using tess_resetTransform_t = void(__fastcall*)(Tessellator*, bool);
		static tess_resetTransform_t tess_resetTransform = reinterpret_cast<tess_resetTransform_t>(findSig(Sigs::tessellator::resetTransform));
		tess_resetTransform(this, a1);
	}

	inline void renderMeshImmediately(ScreenContext* ctx, MaterialPtr* material) {
		using meshHelper_renderImm_t = void(__fastcall*)(ScreenContext*, Tessellator*, MaterialPtr*);
		static meshHelper_renderImm_t meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(findSig(Sigs::tessellator::renderMeshImmediately));
		meshHelper_renderImm(ctx, this, material);
	}
};
