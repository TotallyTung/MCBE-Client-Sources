﻿#include "DrawUtils.h"

#include "Module/ModuleManager.h"
#include <Windows.h>
#include "../Utils/Logger.h"
#include "../Utils/TimerUtils.h"

struct MaterialPtr {
	char padding[0x138];
};

using tess_vertex_t = void(__fastcall*)(Tessellator* _this, float v1, float v2, float v3);
using meshHelper_renderImm_t = void(__fastcall*)(__int64, Tessellator* tesselator, MaterialPtr*);

C_MinecraftUIRenderContext* renderCtx;
C_GuiData* guiData;
__int64 screenContext2d;
__int64 game3dContext;
Tessellator* tesselator;
float* colorHolder;
std::shared_ptr<glmatrixf> refdef;
vec2_t fov;
vec2_t screenSize;
vec3_t origin;
float lerpT;
C_TexturePtr* texturePtr = nullptr;

static MaterialPtr* uiMaterial = nullptr;
static MaterialPtr* entityFlatStaticMaterial = nullptr;

tess_vertex_t tess_vertex;
meshHelper_renderImm_t meshHelper_renderImm;
//mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
//Tessellator__initializeFormat_t Tessellator__initializeFormat;

bool hasInitializedSigs = false;
void initializeSigs() {
	
	tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("40 57 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24"));
	meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 49 8B F0 48 8B DA"));
	//mce__VertexFormat__disableHalfFloats = reinterpret_cast<mce__VertexFormat__disableHalfFloats_t>(FindSignature("40 53 48 83 EC ?? 48 8B D9 C7 81 ?? ?? ?? ?? 00 00 00 00 C6 81 ?? ?? ?? ?? 00"));
	//Tessellator__initializeFormat = reinterpret_cast<Tessellator__initializeFormat_t>(FindSignature("48 89 74 24 ?? 57 48 83 EC 20 4C 8B 41 ?? 48 8B FA 4C 2B 41 ?? 48 8B F1 48 83 C1 08 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 F7 E8 48 D1 FA 48 8B C2 48 C1 E8 3F 48 03 D0 48 3B FA"));
	hasInitializedSigs = true;
}

void DrawUtils::setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* gui) {
	if (!hasInitializedSigs)
		initializeSigs();
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

	ElapsedMicroseconds.QuadPart *= 1000000;
	int ticksPerSecond = 20;
	if (g_Data.getClientInstance()->minecraft != nullptr && reinterpret_cast<__int64>(g_Data.getClientInstance()->minecraft) < 0xFFFFFFFFFFF00000)
		if (g_Data.getClientInstance()->minecraft->timer != nullptr)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
	if(ticksPerSecond < 1)
		ticksPerSecond = 1;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	screenContext2d = reinterpret_cast<__int64*>(renderCtx)[2];

	tesselator = *reinterpret_cast<Tessellator**>(screenContext2d + 0xB0);
	colorHolder = *reinterpret_cast<float**>(screenContext2d + 0x30);

	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = g_Data.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if (uiMaterial == nullptr) {
		// 2 Sigs, wanted one comes first
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		uiMaterial = reinterpret_cast<MaterialPtr*>(sigOffset + offset + 7);
	}
	if (entityFlatStaticMaterial == nullptr && g_Data.isInGame()) {
		entityFlatStaticMaterial = reinterpret_cast<MaterialPtr*>(g_Data.getClientInstance()->itemInHandRenderer->entityLineMaterial.materialPtr);
	}
}

void DrawUtils::setColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

C_Font* DrawUtils::getFont(Fonts font) {
	if (true) {
		auto FontChangerMods = moduleMgr->getModule<FontChanger>();
		if (FontChangerMods->font == 8) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[8].font;
		}
		if (FontChangerMods->font == 7) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[7].font;
		}
		if (FontChangerMods->font == 6) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[6].font;
		}
		if (FontChangerMods->font == 5) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[5].font;
		}
		if (FontChangerMods->font == 4) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[4].font;
		}
		if (FontChangerMods->font == 3) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[3].font;
		}
		if (FontChangerMods->font == 2) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[2].font;
		}
		if (FontChangerMods->font == 1) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[1].font;
		}
		if (FontChangerMods->font == 0) {
			return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[0].font;
		}
		switch (font) {
		case Fonts::SMOOTH:
			return g_Data.getClientInstance()->minecraftGame->getTheGoodFontThankYou();
			break;
		case Fonts::UNICOD:
			return g_Data.getClientInstance()->minecraftGame->getTheBetterFontYes();
			break;
		case Fonts::RUNE:
			return g_Data.getClientInstance()->_getRuneFont();
			break;
		default:
			return g_Data.getClientInstance()->_getFont();
			break;
		}
	}
}


Tessellator* DrawUtils::get3dTessellator() {
	auto myTess = *reinterpret_cast<Tessellator**>(game3dContext + 0xB0);
	return myTess;
}

__int64 DrawUtils::getScreenContext() {
	return game3dContext == 0 ? screenContext2d : game3dContext;
}

MatrixStack* DrawUtils::getMatrixStack() {
	return reinterpret_cast<MatrixStack*>(*reinterpret_cast<__int64*>(DrawUtils::getScreenContext() + 0x18i64) + 0x30i64);
}

float DrawUtils::getTextWidth(std::string* textStr, float textSize, Fonts font) {
	TextHolder text(*textStr);

	C_Font* fontPtr = getFont(font);

	float ret = renderCtx->getLineLength(fontPtr, &text, textSize);

	return ret;
}

float DrawUtils::getFontHeight(float textSize, Fonts font) {
	C_Font* fontPtr = getFont(font);

	float ret = fontPtr->getLineHeight() * textSize;

	return ret;
}

void DrawUtils::flush() {
	renderCtx->flushText(0);
}

void DrawUtils::drawTriangle(const vec2_t& p1, const vec2_t& p2, const vec2_t& p3) {
	
	DrawUtils::tess__begin(tesselator, 3, 3);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}


void DrawUtils::drawQuad(const vec2_t& p1, const vec2_t& p2, const vec2_t& p3, const vec2_t& p4) {
	DrawUtils::tess__begin(tesselator, 1, 4);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);
	tess_vertex(tesselator, p4.x, p4.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawLine(const vec2_t& start, const vec2_t& end, float lineWidth) {
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	DrawUtils::tess__begin(tesselator, 3, 6);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawText(const vec2_t& pos, std::string* textStr, const MC_Color& color, float textSize, float alpha, Fonts font) {
	TextHolder text(*textStr);
	C_Font* fontPtr = getFont(font);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;


	float posF[4];  // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y - 1;
	posF[3] = pos.y + 1000;

	TextMeasureData textMeasure{};
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;

	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}
void DrawUtils::drawCenteredString(vec2_t pos, std::string* textStr, float textSize, MC_Color color) {
	DrawUtils::drawText(vec2_t(pos.x - DrawUtils::getTextWidth(textStr, textSize) / 2.F, pos.y - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.F), textStr, color, textSize, 1);
}
void DrawUtils::fillRectangleA(vec4_t pos, MC_Color col) {
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}
void DrawUtils::drawSteve(vec4_t(pos)) {
	// line 1
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y, pos.x + 3.5f, pos.y + 3.5f), MC_Color(39, 27, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y, pos.x + 7.5f, pos.y + 3.5f), MC_Color(36, 25, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y, pos.x + 11.f, pos.y + 3.5f), MC_Color(39, 26, 13, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y, pos.x + 15.f, pos.y + 3.5f), MC_Color(33, 23, 9, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y, pos.x + 18.5f, pos.y + 3.5f), MC_Color(30, 20, 7, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y, pos.x + 22.5f, pos.y + 3.5f), MC_Color(32, 22, 8, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y, pos.x + 26.f, pos.y + 3.5f), MC_Color(36, 25, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y, pos.x + 30.f, pos.y + 3.5f), MC_Color(35, 24, 11, 255));
	// line 2
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 3.5f, pos.x + 7.5f, pos.y + 7.5f), MC_Color(35, 24, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 3.5f, pos.x + 11.f, pos.y + 7.5f), MC_Color(36, 25, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 3.5f, pos.x + 15.f, pos.y + 7.5f), MC_Color(43, 30, 14, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 3.5f, pos.x + 18.5f, pos.y + 7.5f), MC_Color(55, 35, 15, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 3.5f, pos.x + 22.5f, pos.y + 7.5f), MC_Color(53, 35, 17, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 3.5f, pos.x + 26.f, pos.y + 7.5f), MC_Color(37, 25, 12, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 3.5f, pos.x + 30.f, pos.y + 7.5f), MC_Color(27, 18, 8, 255));
	// line 3
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 7.5f, pos.x + 3.5f, pos.y + 11.f), MC_Color(36, 25, 11, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 7.5f, pos.x + 7.5f, pos.y + 11.f), MC_Color(152, 114, 90, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 7.5f, pos.x + 11.f, pos.y + 11.f), MC_Color(152, 114, 90, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 7.5f, pos.x + 15.f, pos.y + 11.f), MC_Color(158, 118, 95, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 7.5f, pos.x + 18.5f, pos.y + 11.f), MC_Color(165, 125, 107, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 7.5f, pos.x + 22.5f, pos.y + 11.f), MC_Color(158, 116, 95, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 7.5f, pos.x + 26.f, pos.y + 11.f), MC_Color(143, 98, 75, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 7.5f, pos.x + 30.f, pos.y + 11.f), MC_Color(43, 31, 15, 255));
	// line 4
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 11.f, pos.x + 3.5f, pos.y + 15.f), MC_Color(142, 104, 85, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 11.f, pos.x + 7.5f, pos.y + 15.f), MC_Color(150, 110, 91, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 11.f, pos.x + 11.f, pos.y + 15.f), MC_Color(150, 110, 91, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 11.f, pos.x + 15.f, pos.y + 15.f), MC_Color(142, 104, 85, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 11.f, pos.x + 18.5f, pos.y + 15.f), MC_Color(130, 95, 77, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 11.f, pos.x + 22.5f, pos.y + 15.f), MC_Color(156, 114, 95, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 11.f, pos.x + 26.f, pos.y + 15.f), MC_Color(130, 88, 63, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 11.f, pos.x + 30.f, pos.y + 15.f), MC_Color(130, 88, 63, 255));
	// line 5
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 15.f, pos.x + 3.5f, pos.y + 18.5f), MC_Color(150, 110, 91, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 15.f, pos.x + 7.5f, pos.y + 18.5f), MC_Color(213, 213, 213, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 15.f, pos.x + 11.f, pos.y + 18.5f), MC_Color(68, 51, 114, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 15.f, pos.x + 15.f, pos.y + 18.5f), MC_Color(151, 103, 86, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 15.f, pos.x + 18.5f, pos.y + 18.5f), MC_Color(156, 114, 95, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 15.f, pos.x + 22.5f, pos.y + 18.5f), MC_Color(68, 51, 114, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 15.f, pos.x + 26.f, pos.y + 18.5f), MC_Color(213, 213, 213, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 15.f, pos.x + 30.f, pos.y + 18.5f), MC_Color(142, 104, 85, 255));
	// line 6
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 18.5f, pos.x + 3.5f, pos.y + 22.5f), MC_Color(130, 83, 58, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 18.5f, pos.x + 7.5f, pos.y + 22.5f), MC_Color(149, 103, 82, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 18.5f, pos.x + 11.f, pos.y + 22.5f), MC_Color(153, 103, 95, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 18.5f, pos.x + 15.f, pos.y + 22.5f), MC_Color(88, 53, 40, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 18.5f, pos.x + 18.5f, pos.y + 22.5f), MC_Color(88, 53, 40, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 18.5f, pos.x + 22.5f, pos.y + 22.5f), MC_Color(159, 113, 90, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 18.5f, pos.x + 26.f, pos.y + 22.5f), MC_Color(135, 88, 59, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 18.5f, pos.x + 30.f, pos.y + 22.5f), MC_Color(107, 69, 43, 255));
	// line 7
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 22.5f, pos.x + 3.5f, pos.y + 26.f), MC_Color(120, 78, 56, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 22.5f, pos.x + 7.5f, pos.y + 26.f), MC_Color(125, 79, 53, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 22.5f, pos.x + 11.f, pos.y + 26.f), MC_Color(99, 55, 44, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 22.5f, pos.x + 15.f, pos.y + 26.f), MC_Color(99, 55, 44, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 22.5f, pos.x + 18.5f, pos.y + 26.f), MC_Color(99, 55, 44, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 22.5f, pos.x + 22.5f, pos.y + 26.f), MC_Color(99, 55, 44, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 22.5f, pos.x + 26.f, pos.y + 26.f), MC_Color(119, 78, 52, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 22.5f, pos.x + 30.f, pos.y + 26.f), MC_Color(108, 69, 48, 255));
	// line 8
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y + 26.f, pos.x + 3.5f, pos.y + 30.5f), MC_Color(93, 58, 37, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 3.5f, pos.y + 26.f, pos.x + 7.5f, pos.y + 30.5f), MC_Color(91, 56, 35, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 7.5f, pos.y + 26.f, pos.x + 11.f, pos.y + 30.5f), MC_Color(108, 69, 48, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 11.f, pos.y + 26.f, pos.x + 15.f, pos.y + 30.5f), MC_Color(108, 69, 48, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 15.f, pos.y + 26.f, pos.x + 18.5f, pos.y + 30.5f), MC_Color(102, 65, 43, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 18.5f, pos.y + 26.f, pos.x + 22.5f, pos.y + 30.5f), MC_Color(109, 71, 49, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 22.5f, pos.y + 26.f, pos.x + 26.f, pos.y + 30.5f), MC_Color(109, 71, 49, 255));
	DrawUtils::fillRectangleA(vec4_t(pos.x + 26.f, pos.y + 26.f, pos.x + 30.f, pos.y + 30.5f), MC_Color(102, 65, 43, 255));
}
MC_Color DrawUtils::getRainbowWave(float speedInSeconds, float s, float v, long index) {
	auto hudModule = moduleMgr->getModule<HudModule>();
	float hue = ((TimerUtils::getCurrentMs() + index) % (int)(((int)speedInSeconds) * 1000)) / (float)(((int)speedInSeconds) * 1000);
	float r, g, b = 0;
	Utils::ColorConvertHSVtoRGB(hue, 1.f, v, r, g, b);
	return MC_Color(true, r * 255, g * 255, b * 255, 255);
}
MC_Color DrawUtils::getWaveRGB(int red1, int green1, int blue1, int red2, int green2, int blue2, long index) {
	double offset = ((TimerUtils::getCurrentMs() - index) / 7) / (double)120;

	double aids123 = ((TimerUtils::getCurrentMs() - index) % 1000 / 1000.000);
	int aids1234 = ((TimerUtils::getCurrentMs() - index) % 2000 / 2000.000) * 2;

	aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
	double inverse_percent = 1 - aids123;
	int redPart = (int)(red1 * inverse_percent + red2 * aids123);
	int greenPart = (int)(green1 * inverse_percent + green2 * aids123);
	int bluePart = (int)(blue1 * inverse_percent + blue2 * aids123);
	return MC_Color(redPart, greenPart, bluePart);
}
void DrawUtils::drawBox(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool outline) {
	
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int) screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x; 
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y; 
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z; 
				
				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw
	
	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}
		
		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawImage(std::string FilePath, vec2_t& imagePos, vec2_t& ImageDimension, vec2_t& idk) {
	if (texturePtr == nullptr) {
		texturePtr = new C_TexturePtr();
		C_FilePath file(FilePath);
		renderCtx->getTexture(texturePtr, file);
	}

	__int64 yot = 0;
	static unsigned __int64 hashedString = 0xA99285D21E94FC80;
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 0F 29 70 ?? 0F 29 78 ?? 44 0F 29 40 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 4D 8B E1 44 0F 28 C2 4C 8B F2 4C 8B F9");
	// TODO: try 48 8B C4 48 89 58 10 48 89 70 18 55 57 41 54 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 C8 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4D 8B E1
	if (texturePtr != nullptr) {
		renderCtx->drawImage(texturePtr, imagePos, ImageDimension, yot, idk);
		MC_Color col(1.f, 1.f, 1.f);
		renderCtx->flushImages(col, flushImageAddr, (__int64)&hashedString);
	}
}

void DrawUtils::drawTextInWorld(std::string* textToSay, const vec3_t& location, float tsize, vec3_ti tColor, vec3_ti bgColor, float opacity) {
	vec2_t textPos;
	vec4_t rectPos;
	
	float textWidth = getTextWidth(textToSay, tsize);
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * tsize;

	vec3_t actualLocation = location.add(0.5f);
	
	if (refdef->OWorldToScreen(origin, actualLocation, textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * tsize;
		rectPos.y = textPos.y - 1.f * tsize;
		rectPos.z = textPos.x + textWidth + 1.f * tsize;
		rectPos.w = textPos.y + textHeight + 2.f * tsize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * tsize;
		fillRectangle(rectPos, MC_Color(bgColor.x, bgColor.y, bgColor.z), opacity);

		drawText(textPos, textToSay, MC_Color(tColor.x, tColor.y, tColor.z), tsize);
	}
}

std::string CODMW6969(std::string STUFF, int digits) {
	bool afterDecimalPt = false;
	int dp = 0;
	std::string toOutput;

	for (int i = 0; i < STUFF.length(); ++i) {
		if (STUFF.at(i) == '.') {
			afterDecimalPt = true;
		}

		toOutput.append(std::string(1, STUFF.at(i)));

		if (afterDecimalPt) {
			dp++;

			if (dp > digits)
				break;
		}
	}

	return toOutput;
}
void DrawUtils::drawNameTags(C_Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	static auto ThemeMod = moduleMgr->getModule<Theme>();
	vec2_t textPos;
	vec4_t rectPos;
	std::string text = ent->getNameTag()->getText();
	//std::string amount = std::to_string((int)g_DatagetLocalPlayer()->getSelectedItem()->count());
	std::string gamerText = " [" + std::to_string((int)ent->getArmorValue()) + "]";
	text = Utils::sanitize(text);
	//amount = (int)g_Data.getLocalPlayer()->getSelectedItem()->count;
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();
	
	float textWidth = getTextWidth(&text, textSize);
	auto newText = text + gamerText;
	if (nameTagsMod->drawhealth)
		textWidth = getTextWidth(&newText, textSize);

	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * textSize;
	
	if (refdef->OWorldToScreen(origin, ent->eyePos0.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * textSize;
		rectPos.y = textPos.y - 1.f * textSize;
		rectPos.z = textPos.x + textWidth + 1.f * textSize;
		rectPos.w = textPos.y + textHeight + 2.f * textSize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * textSize;

		int a = 0;
		int b = 0;
		int c = 0;
		static float rcolors[4];
		static float disabledRcolors[4];
		static float currColor[4];
		{
			Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;
		}
		c++;
		b++;
		if (b < 20)
			a = moduleMgr->getEnabledModuleCount() * 2;
		else
			b = 0;
		currColor[3] = rcolors[3];
		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
		currColor[0] += 80.f / a * c;
		Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

		static auto nametagsMod = moduleMgr->getModule<NameTags>();


		drawRectangle(rectPos, MC_Color(255, 50, 255), 0.4f, nametagsMod->outline);
	
		drawRectangle(rectPos, MC_Color(255, 50, 255), 0.4f, nametagsMod->outline);

		if (nametagsMod->underline) {
			fillRectangle(subRectPos, MC_Color(255, 50, 255), 0.9f);
		}

		fillRectangle(rectPos, MC_Color(5, 5, 5), nametagsMod->opacity);

		fillRectangle(subRectPos, MC_Color(255,50,255), nametagsMod->outline);
		

		if (nameTagsMod->drawhealth) {
			if (ent->getArmorValue() >= 15.f)
				gamerText = (std::string)GREEN + gamerText;
			else if (ent->getArmorValue() >= 10.f && ent->getArmorValue() < 15.f)
				gamerText = (std::string)YELLOW + gamerText;
			else if (ent->getArmorValue() >= 5.f && ent->getArmorValue() < 10.f)
				gamerText = (std::string)RED + gamerText;
			else if (ent->getArmorValue() > 0.f && ent->getArmorValue() < 5.f)
				gamerText = (std::string)DARK_RED + gamerText;

			auto newText = text + gamerText;
			drawText(textPos, &newText, (FriendList::findPlayer(ent->getNameTag()->getText()) ? (MC_Color(80, 255, 255)) : MC_Color(255, 255, 255)), textSize);
		} else
			drawText(textPos, &text, (FriendList::findPlayer(ent->getNameTag()->getText()) ? (MC_Color(80, 255, 255)) : MC_Color(255, 255, 255)), textSize);
		
		if (ent->isAlive() && ent->isPlayer() && nametagsMod->displayArmor) {  // is player, show armor
			auto* player = reinterpret_cast<C_Player*>(ent);
			float scale = textSize * 0.6f;
			float spacing = scale + 15.f;
			float x = rectPos.x + 1.f * scale;
			float y = rectPos.y - 20.f * scale;
			float y1 = rectPos.y - 28.f * scale;
			// armor
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
					C_Item* item = stack->getItem();
					if (item->getMaxDamage() > 0) {
						int armorDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
						std::string armorDamageText = std::to_string(armorDamage);
						int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
						DrawUtils::drawText(vec2_t(x, y1), &armorDamageText, MC_Color(255 - i, i, 100), 0.7f, 1.f);
						x += scale * spacing;
					}
					x += scale * spacing;
				}

				// item
				C_PlayerInventoryProxy* supplies = player->getSupplies();
				C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
				if (item->isValid()) {
					DrawUtils::drawItem(item, vec2_t(rectPos.z - 1.f - 15.f * scale, y), 1.f, scale, item->isEnchanted());
					C_Item* item = stack->getItem();
					/*if (item->getMaxDamage() > 0) {
						int itemDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
						std::string itemDamageText = std::to_string(itemDamage);
						int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
						DrawUtils::drawText(vec2_t(rectPos.z - 1.f - 15.f * scale, y1), &itemDamageText, MC_Color(255 - i, i, 100), 0.7f, 1.f);
					}*/
				}
			}
		}
	}
}

void DrawUtils::drawEntityBox(C_Entity* ent, float lineWidth) {
	vec3_t end = ent->eyePos0;
	AABB render(end, ent->width, ent->height, end.y - ent->aabb.lower.y);
	render.upper.y += 0.1f;

	drawBox(render.lower, render.upper, lineWidth, true);
}

void DrawUtils::draw2D(C_Entity* ent, float lineWidth) {
	vec3_t base = ent->eyePos0;
	float ofs = (g_Data.getLocalPlayer()->yaw + 90.f) * (PI / 180);

	vec3_t corners[4];
	vec2_t corners2d[4];

	corners[0] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z - ent->width / 1.5f * cos(ofs));
	corners[1] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z + ent->width / 1.5f * cos(ofs));
	corners[2] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z - ent->width / 1.5f * cos(ofs));
	corners[3] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z + ent->width / 1.5f * cos(ofs));

	if (refdef->OWorldToScreen(origin, corners[0], corners2d[0], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[1], corners2d[1], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[2], corners2d[2], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[3], corners2d[3], fov, screenSize)) {
		//float length = (corners2d[1].x - corners2d[0].x) / 4.f;

		/*drawLine(corners2d[0], vec2_t(corners2d[0].x + length, corners2d[0].y), lineWidth);
		drawLine(corners2d[0], vec2_t(corners2d[0].x, corners2d[0].y + length), lineWidth);

		drawLine(vec2_t(corners2d[1].x - length, corners2d[1].y), corners2d[1], lineWidth);
		drawLine(corners2d[1], vec2_t(corners2d[1].x, corners2d[1].y + length), lineWidth);

		drawLine(vec2_t(corners2d[2].x, corners2d[2].y - length), corners2d[2], lineWidth);
		drawLine(corners2d[2], vec2_t(corners2d[2].x + length, corners2d[2].y), lineWidth);

		drawLine(vec2_t(corners2d[3].x, corners2d[3].y - length), corners2d[3], lineWidth);
		drawLine(vec2_t(corners2d[3].x - length, corners2d[3].y), corners2d[3], lineWidth);*/

		drawLine(corners2d[0], corners2d[1], lineWidth);
		drawLine(corners2d[0], corners2d[2], lineWidth);
		drawLine(corners2d[3], corners2d[1], lineWidth);
		drawLine(corners2d[3], corners2d[2], lineWidth);
	}
}

void DrawUtils::drawItem(C_ItemStack* item, const vec2_t& itemPos, float opacity, float scale, bool isEnchanted) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<C_ScreenContext*>(scnCtx);
	C_BaseActorRenderContext baseActorRenderCtx(screenCtx, g_Data.getClientInstance(), g_Data.getClientInstance()->minecraftGame);
	C_ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
}

vec2_t DrawUtils::worldToScreen(const vec3_t& world) {
	vec2_t ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
void DrawUtils::drawLine3d(const vec3_t& start, const vec3_t& end) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 2);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	tess_vertex(myTess, start1.x, start1.y, start1.z);
	tess_vertex(myTess, end1.x, end1.y, end1.z);

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::drawBox3d(const vec3_t& lower, const vec3_t& upper) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 12);

	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	auto newLower = lower.sub(origin);

	vec3_t vertices[8];
	vertices[0] = vec3_t(newLower.x, newLower.y, newLower.z);
	vertices[1] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z);
	vertices[2] = vec3_t(newLower.x, newLower.y, newLower.z + diff.z);
	vertices[3] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

	vertices[4] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z);
	vertices[5] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
	vertices[6] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
	vertices[7] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);

	#define line(m, n) tess_vertex(myTess, m.x, m.y, m.z); \
		tess_vertex(myTess, n.x, n.y, n.z);
	
	#define li(m, n) line(vertices[m], vertices[n]);

	li(0, 1);
	li(1, 3);
	li(3, 2);
	li(2, 0);

	li(4, 5);
	li(5, 7);
	li(7, 6);
	li(6, 4);

	li(0, 4);
	li(1, 5);
	li(2, 6);
	li(3, 7);

	#undef li
	#undef line
	
	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::fillRectangle(const vec4_t& pos, const MC_Color& col, float alpha) {
	DrawUtils::setColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}
inline void DrawUtils::tess__begin(Tessellator* tess, int vertexFormat, int numVerticesReserved) {
	__int64 tesselator = reinterpret_cast<__int64>(tess);

	using tess_begin_t = void(__fastcall*)(Tessellator*, int, int);
	static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(FindSignature("48 89 5C 24 ?? 55 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 45"));
	tess_begin(tess, vertexFormat, numVerticesReserved);
}
void DrawUtils::setGameRenderContext(__int64 ctx) {
	game3dContext = ctx;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if(ctx){
		LARGE_INTEGER EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

		ElapsedMicroseconds.QuadPart *= 1000000;
		int ticksPerSecond = 20;
		if(g_Data.getClientInstance()->minecraft)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
		if(ticksPerSecond < 1)
			ticksPerSecond = 1;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
		lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
		if (lerpT > 1)
			lerpT = 1;
		else if (lerpT < 0)
			lerpT = 0;
	}
}
float DrawUtils::getLerpTime() {
	return lerpT;
}
vec3_t DrawUtils::getOrigin() {
	return origin;
}
void DrawUtils::drawLinestrip3d(const std::vector<vec3_t>& points) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 5, (int)points.size());

	/*
	 * 1: quads
	 * 2: triangle list
	 * 3: trianglestrip (6)
	 * 4: line list
	 * 5: line strip (7)
	 */

	for(const auto& p : points){
		auto pD = p.sub(origin);
		tess_vertex(myTess, pD.x, pD.y, pD.z);
	}
	

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}

MC_Color MC_Color::lerp(const MC_Color& o, float t) const {
	return MC_Color(Utils::lerp(r, o.r, t), Utils::lerp(g, o.g, t), Utils::lerp(b, o.b, t), Utils::lerp(a, o.a, t));
}
