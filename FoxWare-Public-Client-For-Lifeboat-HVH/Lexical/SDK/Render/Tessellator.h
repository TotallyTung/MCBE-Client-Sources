#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../../Utils/Maths.h"

enum class VertextFormat {
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

class Tessellator {
public:
	inline void begin(VertextFormat format, int maxVertices = 0) {
		using tess_begin_t = void(__thiscall*)(Tessellator*, VertextFormat, int);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(Addresses::Tessellator_begin);
		tess_begin(this, format, maxVertices);
	}

	inline void vertex(float x, float y, float z) {
		using tess_vertex_t = void(__thiscall*)(Tessellator*, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(Addresses::Tessellator_vertex);
		tess_vertex(this, x, y, z);
	}

	inline void color(float r, float g, float b, float a) {
		using tess_color_t = void(__thiscall*)(Tessellator*, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(Addresses::Tessellator_color);
		tess_color(this, r, g, b, a);
	}
};