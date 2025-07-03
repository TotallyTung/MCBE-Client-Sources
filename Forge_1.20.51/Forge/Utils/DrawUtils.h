#pragma once
//#include "../SDK/Maths/UIColor.h"
//#include "../SDK/Maths/Vectors.h"
#include "../SDK/Struct/MinecraftUIRenderContext.h"
MinecraftUIRenderContext* ctx;

class DrawUtils {
public:
	static void Initialize(MinecraftUIRenderContext* _ctx) {
		static bool inited = false;
		if (inited) return;
		ctx = _ctx;
		if (ctx != nullptr) inited = true;
	};

	static void fillRectangle(Vector4<float> pos, UIColor col) {
		ctx->fillRectangle(pos, col, col.a);
	};

	static void drawRectangle(Vector4<float> pos, UIColor col, int width) {
		ctx->drawRectangle(pos, col, col.a, width);
	};
};