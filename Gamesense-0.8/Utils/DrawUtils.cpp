﻿#include "DrawUtils.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <glm/ext/matrix_transform.hpp>
#include "../Utils/Logger.h"
#include "../Utils/TimerUtil.h"
#include "../Gamesense/Module/ModuleManager.h"

struct MaterialPtr
{
	char padding[0x138];
};

using tess_vertex_t = void(__fastcall *)(Tessellator *_this, float v1, float v2, float v3);
using meshHelper_renderImm_t = void(__fastcall *)(__int64, Tessellator *tesselator, MaterialPtr *);

C_MinecraftUIRenderContext *renderCtx;
C_GuiData *guiData;
__int64 screenContext2d;
__int64 game3dContext;
Tessellator *tesselator;
float *colorHolder;
std::shared_ptr<glmatrixf> refdef;
vec2_t fov;
vec2_t screenSize;
vec3_t origin;
float lerpT;
C_TexturePtr *texturePtr = nullptr;

static MaterialPtr *uiMaterial = nullptr;
static MaterialPtr *entityFlatStaticMaterial = nullptr;
static MaterialPtr *blendMaterial = nullptr;

tess_vertex_t tess_vertex;
meshHelper_renderImm_t meshHelper_renderImm;
// mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
// Tessellator__initializeFormat_t Tessellator__initializeFormat;

bool hasInitializedSigs = false;
void initializeSigs()
{
	tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("40 57 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24"));
	meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 49 8B F0 48 8B DA"));
	hasInitializedSigs = true;
}

void DrawUtils::setCtx(C_MinecraftUIRenderContext *ctx, C_GuiData *gui)
{
	if (!hasInitializedSigs)
		initializeSigs();
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

	ElapsedMicroseconds.QuadPart *= 1000000;
	int ticksPerSecond = 20;
	if (g_Data.getClientInstance()->minecraft)
		if (g_Data.getClientInstance()->minecraft->timer != nullptr)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
	if (ticksPerSecond < 1)
		ticksPerSecond = 1;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	screenContext2d = reinterpret_cast<__int64 *>(renderCtx)[2];

	tesselator = *reinterpret_cast<Tessellator **>(screenContext2d + 0xB0);
	colorHolder = *reinterpret_cast<float **>(screenContext2d + 0x30);

	glmatrixf *badrefdef = g_Data.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = g_Data.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if (uiMaterial == nullptr)
	{
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		int offset = *reinterpret_cast<int *>(sigOffset + 3);
		uiMaterial = reinterpret_cast<MaterialPtr *>(sigOffset + offset + 7);
	}
	if (entityFlatStaticMaterial == nullptr && g_Data.isInGame())
	{
		entityFlatStaticMaterial = reinterpret_cast<MaterialPtr *>(g_Data.getClientInstance()->itemInHandRenderer->entityLineMaterial.materialPtr);
	}
}

void DrawUtils::setColor(float r, float g, float b, float a)
{
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t *>(colorHolder + 4) = 1;
}

C_Font *DrawUtils::getFont(Fonts font)
{
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	auto cs = moduleMgr->getModule<ClientSetting>();
	if (clickGUI->isEnabled() && clickGUI->cFont) return g_Data.getClientInstance()->minecraftGame->getTheGoodFontThankYou();
	else if (clickGUI->isEnabled() && !clickGUI->cFont) return g_Data.getClientInstance()->minecraftGame->mcFont;
	if (cs->ejectMsg.getSelectedValue() == 1)
		clickGUI->cFont = true;
	if (cs->ejectMsg.getSelectedValue() == 0)
	return g_Data.getClientInstance()->minecraftGame->mcFont;
	if (cs->ejectMsg.getSelectedValue() == 1)
		return g_Data.getClientInstance()->minecraftGame->getTheGoodFontThankYou();
}

Tessellator *DrawUtils::get3dTessellator()
{
	auto myTess = *reinterpret_cast<Tessellator **>(game3dContext + 0xB0);
	return myTess;
}

__int64 DrawUtils::getScreenContext()
{
	return game3dContext == 0 ? screenContext2d : game3dContext;
}

MatrixStack *DrawUtils::getMatrixStack()
{
	return reinterpret_cast<MatrixStack *>(*reinterpret_cast<__int64 *>(DrawUtils::getScreenContext() + 0x18i64) + 0x30i64);
}

float DrawUtils::getTextWidth(std::string *textStr, float textSize, Fonts font)
{
	TextHolder text(*textStr);

	C_Font *fontPtr = getFont(font);

	float ret = renderCtx->getLineLength(fontPtr, &text, textSize, false);

	return ret;
}

float DrawUtils::getFontHeight(float textSize, Fonts font)
{
	C_Font *fontPtr = getFont(font);

	float ret = fontPtr->getLineHeight() * textSize;

	return ret;
}

void DrawUtils::flush()
{
	renderCtx->flushText(0);
}

void DrawUtils::toUpper(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void DrawUtils::toLower(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void DrawUtils::drawTriangle(vec2_t p1, vec2_t p2, vec2_t p3)
{
	DrawUtils::tess__begin(tesselator, 3, 3);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawQuad(vec2_t p1, vec2_t p2, vec2_t p3, vec2_t p4)
{
	DrawUtils::tess__begin(tesselator, 1, 4);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);
	tess_vertex(tesselator, p4.x, p4.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawLine(vec2_t start, vec2_t end, float lineWidth)
{
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

void DrawUtils::drawAnimatedLine(vec2_t start, vec2_t end, MC_Color col, float alpha, float duration, float width)
{
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= width;
	modY *= width;

	DrawUtils::tess__begin(tesselator, 3, 6);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawText(vec2_t pos, std::string *textStr, MC_Color color, float textSize, float alpha, bool hasShadow, Fonts font)
{
	TextHolder text(*textStr);
	C_Font *fontPtr = getFont(font);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;

	pos.y -= 1;

	float posF[4]; // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y;
	posF[3] = pos.y + 1000;

	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &TextMeasureData(&textSize, hasShadow), &caretMeasureData);
}

void DrawUtils::drawCenteredString(vec2_t pos, std::string *textStr, float textSize, MC_Color color, bool hasShadow)
{
	DrawUtils::drawText(vec2_t(pos.x - DrawUtils::getTextWidth(textStr, textSize) / 2.F, pos.y - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.F), textStr, color, textSize, 1, hasShadow);
}

void DrawUtils::drawRightAlignedString(std::string *textStr, vec4_t pos, MC_Color color, bool hasShadow)
{
	TextHolder text(*textStr);
	std::string test = text.getText();
	float lenth2 = DrawUtils::getTextWidth(&test, 1) + 2;
	vec4_t rectPos = vec4_t(pos.x, pos.y, pos.z, pos.y);
	vec2_t textPos = vec2_t(rectPos.z - lenth2, rectPos.y + 2);
	DrawUtils::drawText(textPos, &test, color, 1, 1, hasShadow);
}

void DrawUtils::drawGradientText(vec2_t pos, std::string *textStr, float textSize, float alpha, bool hasShadow)
{
	auto interface = moduleMgr->getModule<Interface>();
	TextHolder text(*textStr);
	std::string test = text.getText();

	int ind = 0;
	for (char c : test)
	{
		ind++;
		int curIndex = -ind * interface->spacing;
		auto interfaceColor = ColorUtil::interfaceColor(curIndex);
		int charWidth = DrawUtils::getTextWidth(&std::string(&c), textSize);

		DrawUtils::drawText(pos, &std::string(&c), interfaceColor, textSize, 1, true);
		pos.x += charWidth;
	}
}
void DrawUtils::drawwtf(vec3_t lowner, float opacity)
{
	//	auto haha = ColorUtil::interfaceColor(1);
	DrawUtils::setColor(1.f, 0.f, 0.f, opacity);
	vec3_t blockPos = vec3_t(lowner.x, lowner.y, lowner.z);
	vec2_t fillBoxThing1 = DrawUtils::worldToScreen(vec3_t(blockPos).add(1.f, 1.001f, 1.f)); // the -0.01 offset makes holeesp work when ur above it but not below
	vec2_t fillBoxThing2 = DrawUtils::worldToScreen(vec3_t(blockPos).add(1.f, 1.001f, 0.f));
	vec2_t fillBoxThing3 = DrawUtils::worldToScreen(vec3_t(blockPos).add(0.f, 1.001f, 0.f));
	vec2_t fillBoxThing4 = DrawUtils::worldToScreen(vec3_t(blockPos).add(0.f, 1.001f, 1.f));
	DrawUtils::drawQuad(vec2_t(fillBoxThing1), vec2_t(fillBoxThing2), vec2_t(fillBoxThing3), vec2_t(fillBoxThing4));
}
void DrawUtils::draw4Faces(vec3_t lower, vec3_t upper) {
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

	// Convert the vertices to screen coordinates
	std::vector<vec2_t> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.push_back(screen);
		}
	}

	// Return if there are less than four points to draw quads with
	if (screenCords.size() < 8) return;

	// Define the indices of the vertices to use for each quad face
	std::vector<std::tuple<int, int, int, int>> faces = {
		//{0, 1, 3, 2},  // Bottom face
		//{4, 5, 7, 6},  // Top face
		{0, 1, 5, 4},  // Front face
		{2, 3, 7, 6},  // Back face
		{1, 3, 7, 5},  // Right face
		{0, 2, 6, 4}   // Left face
	};

	// Draw the quads to fill the box
	for (auto& face : faces) {
		//drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
		DrawUtils::drawQuad(screenCords[std::get<0>(face)], screenCords[std::get<1>(face)], screenCords[std::get<2>(face)], screenCords[std::get<3>(face)]);
		DrawUtils::drawQuad(screenCords[std::get<3>(face)], screenCords[std::get<2>(face)], screenCords[std::get<1>(face)], screenCords[std::get<0>(face)]);
	}
}
void DrawUtils::drawBox(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool fill, int mode, bool enableLerp, float lerpDuration, float elapsedLerpTime) {
	// Lerp
	vec3_t lerpedLower, lerpedUpper;
	float lerpProgress = elapsedLerpTime / lerpDuration;

	// Handle lerping calculation
	if (enableLerp && lerpProgress > 0 && lerpProgress <= 1) {
		lerpedLower = lower.lerp(upper, lerpProgress);  // Lerping lower vertex
		lerpedUpper = upper.lerp(lower, lerpProgress);  // Lerping upper vertex
	}
	else {
		lerpedLower = lower;
		lerpedUpper = upper;
	}

	// Calculate the dimensions of the box
	vec3_t diff = upper.sub(lerpedLower);

	// Create an array of vertices representing the corners of the box
	vec3_t vertices[8];
	vertices[0] = vec3_t(lerpedLower.x, lerpedLower.y, lerpedLower.z);
	vertices[1] = vec3_t(lerpedLower.x + diff.x, lerpedLower.y, lerpedLower.z);
	vertices[2] = vec3_t(lerpedLower.x, lerpedLower.y + diff.y, lerpedLower.z);
	vertices[3] = vec3_t(lerpedLower.x + diff.x, lerpedLower.y + diff.y, lerpedLower.z);
	vertices[4] = vec3_t(lerpedLower.x, lerpedLower.y, lerpedLower.z + diff.z);
	vertices[5] = vec3_t(lerpedLower.x + diff.x, lerpedLower.y, lerpedLower.z + diff.z);
	vertices[6] = vec3_t(lerpedLower.x, lerpedLower.y + diff.y, lerpedLower.z + diff.z);
	vertices[7] = vec3_t(lerpedLower.x + diff.x, lerpedLower.y + diff.y, lerpedLower.z + diff.z);

	if (fill) {
		// Convert the vertices to screen coordinates
		std::vector<vec2_t> screenCords;
		for (int i = 0; i < 8; i++) {
			vec2_t screen;
			if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
				screenCords.push_back(screen);
			}
		}

		// Return if there are less than four points to draw quads with
		if (screenCords.size() < 8) return;

		// Define the indices of the vertices to use for each quad face
		std::vector<std::tuple<int, int, int, int>> faces = {
			{0, 1, 3, 2},  // Bottom face
			{4, 5, 7, 6},  // Top face
			{0, 1, 5, 4},  // Front face
			{2, 3, 7, 6},  // Back face
			{1, 3, 7, 5},  // Right face
			{0, 2, 6, 4}   // Left face
		};

		// Draw the quads to fill the box
		for (auto face : faces) {
			DrawUtils::drawQuad(screenCords[std::get<0>(face)], screenCords[std::get<1>(face)], screenCords[std::get<2>(face)], screenCords[std::get<3>(face)]);
			DrawUtils::drawQuad(screenCords[std::get<3>(face)], screenCords[std::get<2>(face)], screenCords[std::get<1>(face)], screenCords[std::get<0>(face)]);
		}
	}

	if (mode == 1 || mode == 2) {
		// Convert the vertices to screen coordinates
		std::vector<std::tuple<int, vec2_t>> screenCords;
		for (int i = 0; i < 8; i++) {
			vec2_t screen;
			if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
				screenCords.emplace_back(mode == 2 ? (int)screenCords.size() : i, screen);
			}
		}

		// Return if there are less than two points to draw lines between
		if (screenCords.size() < 2) return;

		switch (mode) {
		case 1: {
			// Draw lines between all pairs of vertices
			for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
				auto from = *it;
				auto fromOrig = vertices[std::get<0>(from)];

				for (auto to : screenCords) {
					auto toOrig = vertices[std::get<0>(to)];

					// Determine if the line should be drawn based on the relative positions of the vertices
					bool shouldDraw = false;
					// X direction
					shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
					// Y direction
					shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
					// Z direction
					shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

					if (shouldDraw) DrawUtils::drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
				}
			}
			return;
			break;
		}
		case 2: {
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
					}
					else if (angle <= -PI) {
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
			return;
			break;
		}
		}
	}
}
void DrawUtils::drawFilledBox(vec3_t pos, bool isAnim, float t)
{
	vec3_t myFill = vec3_t(pos.x, pos.y, pos.z);
	if (!isAnim)
	{
		float rentimer = 1;
		float zero = rentimer / 2;
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
	}
	if (isAnim) // very efficient code
	{
		float rentimer = 0;
		if (t != 0 && t <= 28)
			rentimer = (float)(t / 28.f);
		if (t >= 28)
			rentimer = 1;
		float zero = rentimer / 2;
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{

			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill).add(-zero, -zero, -zero));
			if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
				DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
	}
}
void DrawUtils::drawFilledBox2D(vec3_t pos, float height)
{
	vec3_t myFill2D = vec3_t(pos.x, pos.y, pos.z);
	float rentimer = 1;
	float zero = rentimer / 2;
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, -zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{

		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, -zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{

		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{

		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, -zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, -zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{

		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(zero, zero + height, -zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{

		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(myFill2D).add(-zero, zero + height, -zero));
		if (!((fill1.x <= 0 && fill1.y <= 0) || (fill2.x <= 0 && fill2.y <= 0) || (fill3.x <= 0 && fill3.y <= 0) || (fill4.x <= 0 && fill4.y <= 0)))
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
}
void DrawUtils::drawBoxFilled(const vec3_t& lower, const vec3_t& upper, float scale, bool onUi) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();

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
	// Scale vertices using glm
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	vec3_t newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);  // .add(0.4375f, 0.4375f, 0.4375f) is chest
	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = vec3_t{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
	}

#define line(m, n)                      \
	tess_vertex(myTess, m.x, m.y, m.z); \
	tess_vertex(myTess, n.x, n.y, n.z);

	// Top square
	line(vertices[4], vertices[5]);
	line(vertices[5], vertices[7]);
	line(vertices[7], vertices[6]);
	line(vertices[6], vertices[4]);

	// Bottom Square
	line(vertices[0], vertices[1]);
	line(vertices[1], vertices[3]);
	line(vertices[3], vertices[2]);
	line(vertices[2], vertices[0]);

	// Sides
	line(vertices[0], vertices[4]);
	line(vertices[1], vertices[5]);
	line(vertices[2], vertices[6]);
	line(vertices[3], vertices[7]);

#undef line

	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);
}
void DrawUtils::drawImage(std::string filePath, vec2_t &imagePos, vec2_t &imageDimension, vec2_t &idk, vec2_t &idk2, MC_Color flushColor)
{
	texturePtr = new C_TexturePtr();
	C_FilePath file(filePath);
	renderCtx->getTexture(texturePtr, file);
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1");

	if (texturePtr != nullptr)
	{
		renderCtx->drawImage(texturePtr, imagePos, imageDimension, idk, idk2);
		renderCtx->flushImages(flushColor, flushImageAddr, flushString);
		delete texturePtr;
	}
}
void DrawUtils::drawTextInWorld(std::string *textToSay, const vec3_t &location, float tsize, vec3_ti tColor, vec3_ti bgColor, float opacity)
{
	vec2_t textPos;
	vec4_t rectPos;

	float textWidth = getTextWidth(textToSay, tsize);
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * tsize;

	vec3_t actualLocation = location;
	auto loc = actualLocation.add(0.5f);

	if (refdef->OWorldToScreen(origin, loc, textPos, fov, screenSize))
	{
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
void DrawUtils::drawItemInWorld(C_ItemStack* item, const vec3_t& location, float opacity, float scale, bool isEnchanted)
{
	vec2_t itemPos;
	vec3_t actualLocation = location;

	if (refdef->OWorldToScreen(origin, actualLocation, itemPos, fov, screenSize)) {
		drawItem(item, itemPos, opacity, scale, isEnchanted);
	}
}
void DrawUtils::drawNameTags(C_Entity *ent, float textSize, bool drawHealth, bool useUnicodeFont)
{
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();
	vec2_t textPos;
	vec4_t rectPos;
	std::string text = ent->getNameTag()->getText();
	std::string HealthText = std::to_string((int)ent->getHealth());
	text = Utils::sanitize(text);
	text = text.substr(0, text.find('\n'));

	float textWidth = getTextWidth(&text, textSize);
	if (nameTagsMod->drawhealth) {
		textWidth += getTextWidth(&std::string(" "), textSize) + getTextWidth(&HealthText, textSize);
	}
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * textSize;

	if (refdef->OWorldToScreen(origin, ent->eyePos0.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.5f * textSize;
		rectPos.y = textPos.y - 1.5f * textSize;
		rectPos.z = textPos.x + textWidth + 1.5f * textSize;
		rectPos.w = textPos.y + textHeight + 0.f * textSize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * textSize;
		//auto nametagsMod = moduleMgr->getModule<NameTags>();
		if (ent->damageTime > 1)
			fillRectangleA(rectPos, MC_Color(255, 0, 0, nameTagsMod->opacity));
		else
		fillRectangleA(rectPos, MC_Color(0, 0, 0, nameTagsMod->opacity));
		//if (nametagsMod->underline) {
		//	fillRectangle(subRectPos, MC_Color(255, 255, 255), 1.f);
		//}
		//drawText(textPos, &text, MC_Color(255, 255, 255), textSize, 1.f, true);
		if (FriendList::findPlayer(ent->getNameTag()->getText()) && ent->getNameTag()->getTextLength() >= 1 && ent->getEntityTypeId() == 319) {
			drawText(textPos, &text, MC_Color(0, 255, 255), textSize, 1.f, true);
		}
		else drawText(textPos, &text, MC_Color(255, 255, 255), textSize, 1.f, true);
		if (nameTagsMod->drawhealth) {
			textPos.x += getTextWidth(&text, textSize) + getTextWidth(&std::string(" "), textSize);

			MC_Color green(0, 255, 0);
			MC_Color red(255, 0, 0);

			float fraction = ent->getHealth() / ent->getMaxHealth();

			vec3_t greenVec(0, 0, 0);
			vec3_t redVec(0, 0, 0);

			Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
			Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);

			vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

			MC_Color gamerColor(0, 0, 0);

			Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);

			DrawUtils::drawText(textPos, &HealthText, MC_Color(gamerColor), textSize, 1.f, true);
		}

		//static auto nameTagsMod = moduleMgr->getModule<NameTags>();

		if (ent->getEntityTypeId() == 319 && nameTagsMod->displayArmor) { // animals dont hav amor :rage: (animals abuse)
			auto* player = reinterpret_cast<C_Player*>(ent);
			float scale = textSize * 0.75f;
			float spacing = scale + 15.f;
			float x = 0;
			float y = rectPos.y - 20.f * scale;
			if (player->getEquippedTotem()->item != nullptr) {
				if ((*player->getEquippedTotem()->item)->itemId == 568) {
					x += scale * spacing;
				}
			}
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					x += scale * spacing;
				}
			}
			C_ItemStack* stack = player->getSelectedItem();
			if (stack->item != nullptr) {
				x += scale * spacing;
			}
			float xReal = (rectPos.x + rectPos.z - x) / 2;
			if (player->getOffhandItem()->item != nullptr) {
				if ((*player->getOffhandItem()->item)->itemId == 568) {
					//x -= scale * spacing;
					C_ItemStack* totem = player->getOffhandItem();
					DrawUtils::drawItem(totem, vec2_t(xReal, y), 1.f, scale, false);
					xReal += scale * spacing;
				}
				if ((*player->getOffhandItem()->item)->itemId != 568) {
					C_ItemStack* totem = player->getOffhandItem();
					float dura11 = stack->getItem()->getMaxDamage();
					float dura22 = stack->getItem()->getDamageValue(stack->tag);
					float dura33 = dura11 - dura22;
					int dura44 = dura33 / dura11 * 100;
					std::string durastr1 = std::to_string((int)dura44) + "%";
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura33 / dura11 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec1 = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor1(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec1.x, colorVec1.y, colorVec1.z, gamerColor1.r, gamerColor1.g, gamerColor1.b);
					DrawUtils::drawItem(totem, vec2_t(xReal, y), 1.f, scale, false);
					if (nameTagsMod->showBar)
						DrawUtils::DrawBarItem(totem, vec2_t(xReal, y), scale);
					if (nameTagsMod->showPercentage && !dura11 == 0)
					DrawUtils::drawText(vec2_t(xReal, y), &durastr1, MC_Color(gamerColor1), textSize * 0.3f, 1.f, true);
					xReal += scale * spacing;
				}
			}
			// armor
			for (int i = 0; i < 4; i++) {

				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {

					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4) + "%";
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
					if (nameTagsMod->showPercentage)
					DrawUtils::drawText(vec2_t(xReal, y), &durastr, MC_Color(gamerColor), textSize * 0.3f, 1.f, true);
					DrawUtils::drawItem(stack, vec2_t(xReal, y), 1.f, scale, stack->isEnchanted());
					//stack->count
					if (nameTagsMod->showBar)
					DrawUtils::DrawBarItem(stack, vec2_t(xReal, y), scale);
					if (nameTagsMod->showEnchantment) {
						float enchanty = y + scale * (scale + 5.f);
						for (int i = 0; i <= 38; i++) {
							int enchantvalue = stack->getEnchantValue(i);
							if (enchantvalue > 0) {
								std::string enchantname;
								if (i == 0) enchantname = "Pr";
								if (i == 1) enchantname = "FPr";
								if (i == 2) enchantname = "FF";
								if (i == 3) enchantname = "BPr";
								if (i == 4) enchantname = "PPr";
								if (i == 5) enchantname = "Th";
								if (i == 6) enchantname = "Re";
								if (i == 7) enchantname = "DS";
								if (i == 8) enchantname = "AI";
								if (i == 9) enchantname = "Sh";
								if (i == 10) enchantname = "Sm";
								if (i == 11) enchantname = "BoA";
								if (i == 12) enchantname = "Kb";
								if (i == 13) enchantname = "FA";
								if (i == 14) enchantname = "Lo";
								if (i == 15) enchantname = "Ef";
								if (i == 16) enchantname = "ST";
								if (i == 17) enchantname = "Uk";
								if (i == 18) enchantname = "Ft";
								if (i == 19) enchantname = "Pw";
								if (i == 20) enchantname = "Pu";
								if (i == 21) enchantname = "Fl";
								if (i == 22) enchantname = "Inf";
								if (i == 23) enchantname = "LotS";
								if (i == 24) enchantname = "Lu";
								if (i == 25) enchantname = "FW";
								if (i == 26) enchantname = "Me";
								if (i == 27) enchantname = "CoB";
								if (i == 28) enchantname = "CoV";
								if (i == 29) enchantname = "Im";
								if (i == 30) enchantname = "Rip";
								if (i == 31) enchantname = "Loy";
								if (i == 32) enchantname = "Cha";
								if (i == 33) enchantname = "MSh";
								if (i == 34) enchantname = "Pi";
								if (i == 35) enchantname = "QC";
								if (i == 36) enchantname = "SS";


								std::string enchantstr = enchantname + std::to_string(enchantvalue);
								DrawUtils::drawText(vec2_t(xReal, enchanty), &enchantstr, MC_Color(255, 255, 255), textSize * 0.4f, 1.f, true);
								enchanty += scale * (scale + 5.f);
							}
						}
					}
					xReal += scale * spacing;
				}
			}

			// item
			{
				//C_ItemStack* stack = player->getSelectedItem();
				if (stack->item != nullptr) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4) + "%";
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
					if (!dura1 == 0 && nameTagsMod->showPercentage) DrawUtils::drawText(vec2_t(xReal, y), &durastr, MC_Color(gamerColor), textSize * 0.3f, 1.f, true);
					DrawUtils::drawItem(stack, vec2_t(xReal, y), 1.f, scale, stack->isEnchanted());
					DrawUtils::DrawBarItem(stack, vec2_t(xReal, y), scale);
					if (nameTagsMod->showEnchantment) {
						float enchanty = y + scale * (scale + 5.f);
						for (int i = 0; i <= 38; i++) {
							int enchantvalue = stack->getEnchantValue(i);
							if (enchantvalue > 0) {
								std::string enchantname;
								if (i == 0) enchantname = "Pr";
								if (i == 1) enchantname = "FPr";
								if (i == 2) enchantname = "FF";
								if (i == 3) enchantname = "BPr";
								if (i == 4) enchantname = "PPr";
								if (i == 5) enchantname = "Th";
								if (i == 6) enchantname = "Re";
								if (i == 7) enchantname = "DS";
								if (i == 8) enchantname = "AI";
								if (i == 9) enchantname = "Sh";
								if (i == 10) enchantname = "Sm";
								if (i == 11) enchantname = "BoA";
								if (i == 12) enchantname = "Kb";
								if (i == 13) enchantname = "FA";
								if (i == 14) enchantname = "Lo";
								if (i == 15) enchantname = "Ef";
								if (i == 16) enchantname = "ST";
								if (i == 17) enchantname = "Uk";
								if (i == 18) enchantname = "Ft";
								if (i == 19) enchantname = "Pw";
								if (i == 20) enchantname = "Pu";
								if (i == 21) enchantname = "Fl";
								if (i == 22) enchantname = "Inf";
								if (i == 23) enchantname = "LotS";
								if (i == 24) enchantname = "Lu";
								if (i == 25) enchantname = "FW";
								if (i == 26) enchantname = "Me";
								if (i == 27) enchantname = "CoB";
								if (i == 28) enchantname = "CoV";
								if (i == 29) enchantname = "Im";
								if (i == 30) enchantname = "Rip";
								if (i == 31) enchantname = "Loy";
								if (i == 32) enchantname = "Cha";
								if (i == 33) enchantname = "MSh";
								if (i == 34) enchantname = "Pi";
								if (i == 35) enchantname = "QC";
								if (i == 36) enchantname = "SS";


								std::string enchantstr = enchantname + std::to_string(enchantvalue);
								DrawUtils::drawText(vec2_t(xReal, enchanty), &enchantstr, MC_Color(255, 255, 255), textSize * 0.4f, 1.f, true);
								enchanty += scale * (scale + 5.f);
							}
						}
					}
				}
			}
		}
	}
}
void DrawUtils::DrawBarItem(C_ItemStack* stack, vec2_t ItemPos, float scale) {
	float dura1 = stack->getItem()->getMaxDamage();
	float dura2 = stack->getItem()->getDamageValue(stack->tag);
	float dura3 = dura1 - dura2;
	int dura4 = dura3 / dura1 * 100;
	if (dura3 != dura1) {

		MC_Color green(0, 255, 0);
		MC_Color red(255, 0, 0);
		float dura5 = dura3 / dura1 * 100;
		float fraction = dura5 / 100;
		vec3_t greenVec(0, 0, 0);
		vec3_t redVec(0, 0, 0);
		Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
		Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
		vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

		MC_Color gamerColor(0, 0, 0);

		Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
		vec4_t BarPos = vec4_t(ItemPos.x + 1.5f * scale, ItemPos.y + 14.f * scale, ItemPos.x + 15.f * scale, ItemPos.y + 16.f * scale);
		vec4_t rect = vec4_t(BarPos.x, BarPos.y, BarPos.x + ((BarPos.z - BarPos.x) / 100 * dura4), BarPos.w - 1.f * scale);
		DrawUtils::fillRectangle(BarPos, MC_Color(0, 0, 0), 1.f);
		DrawUtils::fillRectangle(rect, MC_Color(gamerColor), 1.f);
	}
}
void DrawUtils::drawSteve(vec4_t(pos))
{
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

void DrawUtils::drawEntityBox(C_Entity *ent, float lineWidth)
{
	static auto esp = moduleMgr->getModule<ESP>();
	if (esp->mode.getSelectedValue() == 1)
	{
		AABB render(ent->eyePos0, ent->width, ent->height, ent->eyePos0.y - ent->aabb.lower.y);
		render.upper.y += 0.1;
		drawBox(render.lower, render.upper, lineWidth, false);
	}
	else
	{
		AABB render(ent->eyePos0, ent->width, ent->height, ent->eyePos0.y - ent->aabb.lower.y);
		render.upper.y += 0.1;
		drawBox(render.lower, render.upper, lineWidth, true);
	}
}

void DrawUtils::draw2DBox(float width, float height, float lineWidth, float offset, MC_Color color)
{
	DrawUtils::fillRectangle(vec4_t(-width / 2 - offset, -offset, width / 4, lineWidth), color, 1);
	DrawUtils::fillRectangle(vec4_t(width / 2 - offset, -offset, -width / 4, lineWidth), color, 1);
	DrawUtils::fillRectangle(vec4_t(width / 2 - offset, height - offset, -width / 4, lineWidth), color, 1);
	DrawUtils::fillRectangle(vec4_t(-width / 2 - offset, height - offset, width / 4, lineWidth), color, 1);

	DrawUtils::fillRectangle(vec4_t(-width / 2 - offset, height - offset, lineWidth, -height / 4), color, 1);
	DrawUtils::fillRectangle(vec4_t(width / 2 - lineWidth - offset, height - offset, lineWidth, -height / 4), color, 1);
	DrawUtils::fillRectangle(vec4_t(width / 2 - lineWidth - offset, -offset, lineWidth, height / 4), color, 1);
	DrawUtils::fillRectangle(vec4_t(-width / 2 - offset, -offset, lineWidth, height / 4), color, 1);
}

void DrawUtils::draw2D(C_Entity *ent, float lineWidth)
{
	auto pos = ent->getPos();
}

void DrawUtils::drawBetter2D(C_Entity *ent, float lineWidth)
{
	float ofs = (g_Data.getLocalPlayer()->yaw + 90.f) * (PI / 180);

	vec3_t corners[4];
	vec2_t corners2d[4];
	corners[0] = vec3_t(ent->eyePos0.x - ent->width / 1.f * -sin(ofs), ent->aabb.upper.y + (float)0.2, ent->eyePos0.z - ent->width / 1.f * cos(ofs));
	corners[0] = vec3_t(ent->eyePos0.x - ent->width / 1.f * -sin(ofs), ent->aabb.upper.y + (float)0.2, ent->eyePos0.z - ent->width / 1.f * cos(ofs));
	corners[1] = vec3_t(ent->eyePos0.x + ent->width / 1.f * -sin(ofs), ent->aabb.upper.y + (float)0.2, ent->eyePos0.z + ent->width / 1.f * cos(ofs));
	corners[2] = vec3_t(ent->eyePos0.x - ent->width / 1.f * -sin(ofs), ent->aabb.lower.y, ent->eyePos0.z - ent->width / 1.f * cos(ofs));
	corners[3] = vec3_t(ent->eyePos0.x + ent->width / 1.f * -sin(ofs), ent->aabb.lower.y, ent->eyePos0.z + ent->width / 1.f * cos(ofs));

	if (refdef->OWorldToScreen(origin, corners[0], corners2d[0], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[1], corners2d[1], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[2], corners2d[2], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[3], corners2d[3], fov, screenSize))
	{
		float length = (corners2d[1].x - corners2d[0].x) / 10.f;

		float t = 0.3f;
		DrawUtils::drawLine(corners2d[0], corners2d[1], t);
		DrawUtils::drawLine(corners2d[0], corners2d[2], t);
		DrawUtils::drawLine(corners2d[3], corners2d[1], t);
		DrawUtils::drawLine(corners2d[3], corners2d[2], t);
	}
}

void DrawUtils::drawItem(C_ItemStack *item, vec2_t itemPos, float opacity, float scale, bool isEnchanted)
{
	__int64 scnCtx = reinterpret_cast<__int64 *>(renderCtx)[2];
	auto *screenCtx = reinterpret_cast<C_ScreenContext *>(scnCtx);
	C_BaseActorRenderContext baseActorRenderCtx(screenCtx, g_Data.getClientInstance(), g_Data.getClientInstance()->minecraftGame);
	C_ItemRenderer *renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
}
void DrawUtils::drawKeystroke(char key, vec2_t pos)
{
	std::string keyString = Utils::getKeybindName(key);

	C_GameSettingsInput *input = g_Data.getClientInstance()->getGameSettingsInput();
	if (key == *input->spaceBarKey)
		keyString = "-";
	vec4_t rectPos(
		pos.x,
		pos.y,
		pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f),
		pos.y + 20.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(128, 128, 128) : MC_Color(0, 0, 0), 0.2f);
	drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, true);
}

vec2_t DrawUtils::worldToScreen(const vec3_t &world)
{
	vec2_t ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}

void DrawUtils::drawLine3d(const vec3_t &start, const vec3_t &end)
{
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 2);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	tess_vertex(myTess, start1.x, start1.y, start1.z);
	tess_vertex(myTess, end1.x, end1.y, end1.z);

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}

void DrawUtils::drawBox3d(vec3_t lower, vec3_t upper)
{
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 12);

	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	lower = lower.sub(origin);

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);

	vertices[4] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

#define line(m, n)                      \
	tess_vertex(myTess, m.x, m.y, m.z); \
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
void DrawUtils::fillRectangle(vec4_t pos, const MC_Color col, float alpha)
{
	DrawUtils::setColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}

void DrawUtils::drawButton(vec4_t pos, vec2_t textPos, std::string *textStr, const MC_Color hoverColor, MC_Color color)
{
	static bool leftClickDown;
	leftClickDown = g_Data.isLeftClickDown();
	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}
	if (pos.contains(&mousePos) && leftClickDown)
		;
	float buttonLen = DrawUtils::getTextWidth(textStr, 1);
	textPos = vec2_t(pos.x + 2, pos.y);
	DrawUtils::drawText(textPos, textStr, color);
	DrawUtils::drawRoundRectangle(vec4_t(pos.x, pos.y, pos.x + buttonLen, pos.y), pos.contains(&mousePos) ? hoverColor : color, false);
}

void DrawUtils::fillRectangleA(vec4_t pos, MC_Color col)
{
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}

void DrawUtils::drawWarning(vec4_t pos, const MC_Color col, float alpha)
{
	DrawUtils::setColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawLine(vec2_t(pos.w, pos.w), vec2_t(pos.x, pos.y), 0.5f);
	DrawUtils::drawLine(vec2_t(pos.z, pos.w), vec2_t(pos.y, pos.y), 0.5f);
}

void DrawUtils::drawRoundRectangle(vec4_t pos, const MC_Color col, bool rounder = false)
{
	switch (rounder)
	{
	case 0:
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 2, pos.z - 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1, pos.z - 1, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y, pos.x, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y, pos.z + 1, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w, pos.z, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w, pos.x + 1, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 1, pos.z, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 1, pos.x + 1, pos.y), col);
		break;
	case 1:
		DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y, pos.x, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y, pos.z + 1, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.w + 2, pos.z - 2, pos.w + 3), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.y - 3, pos.z - 2, pos.y - 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 2, pos.x + 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 2.5, pos.x + 2, pos.y - 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - .5, pos.y - 1, pos.x + .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 2, pos.z, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 2, pos.y - 2.5, pos.z - 1, pos.y - 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - .5, pos.y - 1, pos.z + .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - .5, pos.w, pos.x + .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w + 1, pos.x + 1, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1.5f, pos.x + 2, pos.w + 2.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - .5, pos.w, pos.z + .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w + 1, pos.z, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 2, pos.w + 1.5f, pos.z - 1, pos.w + 2.5), col);
		break;
	}
}

void DrawUtils::fillRoundRectangle(vec4_t pos, const MC_Color col, bool rounder = false)
{
	switch (rounder)
	{
	case 0:
		DrawUtils::setColor(col.r, col.g, col.b, col.a);
		DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1, pos.z - 1, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w, pos.z - 1, pos.w + 1), col);
		break;
	case 1:
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y, pos.z, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.w, pos.z - .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1, pos.z - 1, pos.w + 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.w + 1.5, pos.z - 2, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.y - 1, pos.z - .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1.5f, pos.z - 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.y - 2, pos.z - 2, pos.y - 1.5), col);
		break;
	}
}

void DrawUtils::drawRoundRectangle2(vec4_t pos, const MC_Color col)
{
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 2, pos.z, pos.y - 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w + 1, pos.z, pos.w + 2), col);

	DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y - 1, pos.x, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y - 1, pos.z + 1, pos.w + 1), col);

	DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w, pos.z, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w, pos.x + 1, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 1, pos.z, pos.y), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 1, pos.x + 1, pos.y), col);
}

void DrawUtils::drawCircle(vec4_t(pos), MC_Color col)
{
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y, pos.z, pos.w), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.w, pos.z - .5, pos.w + .5), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + .5, pos.z - 1, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.y - .5, pos.z - .5, pos.y), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1.f, pos.z - 1, pos.y - .5), col);
}

inline void DrawUtils::tess__begin(Tessellator *tess, int vertexFormat, int numVerticesReserved)
{
	__int64 tesselator = reinterpret_cast<__int64>(tess);
	using tess_begin_t = void(__fastcall *)(Tessellator *, int, int);
	static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(FindSignature("48 89 5C 24 ?? 55 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 45"));
	tess_begin(tess, vertexFormat, numVerticesReserved);
}

void DrawUtils::setGameRenderContext(__int64 ctx)
{
	game3dContext = ctx;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if (ctx)
	{
		LARGE_INTEGER EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

		ElapsedMicroseconds.QuadPart *= 1000000;
		int ticksPerSecond = 20;
		if (g_Data.getClientInstance()->minecraft)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
		if (ticksPerSecond < 1)
			ticksPerSecond = 1;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
		lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
		if (lerpT > 1)
			lerpT = 1;
		else if (lerpT < 0)
			lerpT = 0;
	}
}

int DrawUtils::fadeOut(int startingAlpha, int speed)
{
	return startingAlpha -= speed;
}

float DrawUtils::getLerpTime()
{
	return lerpT;
}

vec3_t DrawUtils::getOrigin()
{
	return origin;
}

void DrawUtils::drawLineStrip3D(const std::vector<vec3_t> &points, int mode)
{
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	/*
	 * 1: quads
	 * 2: triangle list
	 * 3: trianglestrip (6)
	 * 4: line list
	 * 5: line strip (7)
	 */

	auto myTess = DrawUtils::get3dTessellator();
	DrawUtils::tess__begin(myTess, mode, (int)points.size());
	for (const auto &p : points)
	{
		auto pD = p.sub(origin);
		tess_vertex(myTess, pD.x, pD.y, pD.z);
	}

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
