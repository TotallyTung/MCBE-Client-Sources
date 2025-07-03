#pragma once

#include "MaterialPtr.h"

class ScreenContext;

enum class VertextFormat
{
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

struct TessellatorQuadInfo {
public:
    /* this + 0  */ unsigned char facing;
    /* this + 1  */ unsigned char twoFace;
    /* this + 4  */ Vector3<float> centroid;
};
static_assert(sizeof(TessellatorQuadInfo) == 0x10);

class Tessellator {
private:
    char pad_0000[0x184]; // Updated to latest mf
    Vector3<float> transformOffset; // 0x184
public:
	inline void begin(VertextFormat format, int reservedVerticies = 0)
	{
		using tess_begin_t = void(__fastcall*)(Tessellator*, VertextFormat, bool);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(SigManager::Tessellator_begin); // Updated to 1.20.51
		// 48 89 5c 24 ? 55 48 83 ec ? 80 b9 ? ? ? ? ? 45 0f b6 d1 in 1.20.0.1
		tess_begin(this, format, reservedVerticies);
	}

	inline void vertex(float x, float y, float z)
	{
		using tess_vertex_t = void(__fastcall*)(Tessellator*, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(SigManager::Tessellator_vertex); // 1.21.50
		tess_vertex(this, x, y, z);
	}

	inline void vertexUV(float x, float y, float z, float u = 0, float v = 0)
	{
		using tess_vertex_t = void(__fastcall*)(Tessellator*, float, float, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(SigManager::Tessellator_vertexUV); // 1.21.50
		tess_vertex(this, x, y, z, u , v);
	}

	inline void color(float r, float g, float b, float a)
	{
		using tess_color_t = void(__fastcall*)(Tessellator*, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(SigManager::Tessellator_color); // 1.21.50
		tess_color(this, r, g, b, a);
	}

	inline void setRotation(float angle, Vector3<float> pivot)
	{
		using tess_setRotation_t = void(__fastcall*)(Tessellator*, float, Vector3<float>);
		static tess_setRotation_t tess_setRotation = reinterpret_cast<tess_setRotation_t>(SigManager::Tessellator_setRotation); // 1.21.50
		tess_setRotation(this, angle, pivot);
	}

	inline void resetTransform(bool setToIdentity)
	{
		using tess_resetTransform_t = void(__fastcall*)(Tessellator*, bool);
		static tess_resetTransform_t tess_resetTransform = reinterpret_cast<tess_resetTransform_t>(SigManager::Tessellator_resetTransform); // 1.21.50
		tess_resetTransform(this, setToIdentity);
	}

	static uintptr_t offsetFromSig(uintptr_t sig, int offset) { // REL RIP ADDR RESOLVER
		// pointer is relative to the code it is in - it is how far to the left in bytes you need to move to get to the value it points to,
		// this function returns absolute address in memory ("(sig)A B C (+offset)? ? ? ?(+4)(from here + bytes to move to get to value pointer points to) D E F")
		// offset val = *reinterpret_cast<int *>(sig + offset)
		// base = sig + offset + 4

		if (sig == 0) return 0;
		return sig + offset + 4 + *reinterpret_cast<int*>(sig + offset);
	}

	inline void renderMeshImmediately(ScreenContext* ctx, MaterialPtr* material)
	{
		char pad[0x58]{};
		//static auto sig = offsetFromSig((uintptr_t)SigManager::MeshHelpers_renderMeshImmediately, 1);
		//static auto sig = this + 0x20D;
		//using func_t = void(*)(ScreenContext*, Tessellator*, MaterialPtr*, char*);
		//static auto func = reinterpret_cast<func_t>(sig);
		//func(ctx, this, material, pad);

		using meshHelper_renderImm_t = void(__fastcall*)(ScreenContext*, Tessellator*, MaterialPtr*, char*);
		static meshHelper_renderImm_t meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(SigManager::MeshHelpers_renderMeshImmediately); // 1.21.50
		meshHelper_renderImm(ctx, this, material, pad);
	}
};