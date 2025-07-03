#include "DrawUtils.h"

#include "Module/ModuleManager.h"
#include <Windows.h>
#include "../Utils/Logger.h"
#include "../Utils/ClientColors.h"
#include <glm/ext/matrix_transform.hpp>

class MaterialPtr {
	char padding[0x138];
};

using tess_vertex_t = void(__fastcall*)(Tessellator* _this, float v1, float v2, float v3);
using meshHelper_renderImm_t = void(__fastcall*)(__int64, Tessellator* tessellator, MaterialPtr*);

MinecraftUIRenderContext* renderCtx;
MinecraftUIRenderContext2* renderCtx2;
GuiData* guiData;
Font* mcFont;
__int64 screenContext2d;
__int64 game3dContext;
Tessellator* tessellator;
float* colorHolder;
std::shared_ptr<glmatrixf> refdef;
Vec2 fov;
Vec2 screenSize;
Vec3 origin;
TexturePtr* texturePtr = nullptr;
void DrawUtils::fillRectangleA(Vec4 pos, MC_Color col) {
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}

static MaterialPtr* uiMaterial = nullptr;
static MaterialPtr* entityFlatStaticMaterial = nullptr;
static MaterialPtr* blendMaterial = nullptr;

bool DrawUtils::readyToRender = false;
bool DrawUtils::isLeftClickDown = false;
bool DrawUtils::isRightClickDown = false;
bool DrawUtils::shouldToggleLeftClick = false;
bool DrawUtils::shouldToggleRightClick = false;
float DrawUtils::deltaTime = 0.016f;

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
	mcFont = Game.getClientInstance()->minecraftGame->mcFont;
	hasInitializedSigs = true;
}
static __int64 getCurrentMs() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	__int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}
MC_Color rainbowColor(float seconds, float saturation, float brightness, long index) {
	float hue6 = (((getCurrentMs() + index) % (int)(6000)) / (float)(6000));
	float r, g, b = 0;
	Utils::ColorConvertHSVtoRGB(hue6, saturation, brightness, r, g, b);

	return MC_Color(r, g, b);
}
void DrawUtils::drawHat(std::vector<Vec3> posList, float opacity, float height, Vec3 pos, bool rgb) {
	auto tess = DrawUtils::get3dTessellator();
	DrawUtils::tess__begin(tess, 3, (int)posList.size());

	DrawUtils::setColor(1, 1, 1, opacity);
	int num = 0;
	for (const auto& p : posList) {
		MC_Color c = rainbowColor(0, 1.f, 1.f, num * 361 / 10);
		if (rgb) tess->color(c.r, c.g, c.b, opacity);
		auto pD = p.sub(origin);
		tess_vertex(tess, pD.x, pD.y, pD.z);
		tess_vertex(tess, pos.sub(origin).x, pD.y + height, pos.sub(origin).z);
		tess_vertex(tess, pD.x, pD.y, pD.z);
		++num;
	}
	meshHelper_renderImm(game3dContext, tess, uiMaterial);
}

void DrawUtils::drawImage(std::string filePath, Vec2& imagePos, Vec2& imageDimension, Vec2& idk, Vec2& idk2, bool system) {
	texturePtr = new TexturePtr();
	FilePath fileasdasd(filePath, system);

	renderCtx2->getTexture(texturePtr, &fileasdasd, false);
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1");

	if (texturePtr != nullptr) {
		renderCtx2->drawImage(texturePtr, imagePos, imageDimension, idk, idk2);
		renderCtx2->flushImages(MC_Color(1.f, 1.f, 1.f), flushImageAddr, flushString);
		delete texturePtr;
	}
}
void DrawUtils::setCtx(MinecraftUIRenderContext* ctx, GuiData* gui) {
	if (!hasInitializedSigs)
		initializeSigs();
	
	guiData = gui;
	renderCtx = ctx;
	screenContext2d = reinterpret_cast<__int64*>(renderCtx)[2];

	tessellator = *reinterpret_cast<Tessellator**>(screenContext2d + 0xB0);
	colorHolder = *reinterpret_cast<float**>(screenContext2d + 0x30);

	glmatrixf* badrefdef = Game.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = Game.getClientInstance()->getFov();
	screenSize = gui->windowSize;
	if (Game.getClientInstance()->levelRenderer != nullptr)
		origin = Game.getClientInstance()->levelRenderer->getOrigin();

	if (uiMaterial == nullptr) {
		// 2 Sigs, wanted one comes first
		//uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		//int offset = *reinterpret_cast<int*>(sigOffset + 3);
		//uiMaterial = reinterpret_cast<MaterialPtr*>(sigOffset + offset + 7);
		uiMaterial = reinterpret_cast<MaterialPtr*>(new mce::MaterialPtr("ui_fill_color"));
	}
	if (entityFlatStaticMaterial == nullptr) {
		//entityFlatStaticMaterial = reinterpret_cast<MaterialPtr*>(Game.getClientInstance()->itemInHandRenderer->entityLineMaterial.materialPtr);
		entityFlatStaticMaterial = reinterpret_cast<MaterialPtr*>(new mce::MaterialPtr("selection_overlay"));
	}
	if (blendMaterial == nullptr) {
		blendMaterial = reinterpret_cast<MaterialPtr*>(new mce::MaterialPtr("fullscreen_cube_overlay_blend"));
	}
	readyToRender = true;
}

void DrawUtils::updateGameDeltaTime(ScreenView* screenView) {
	DrawUtils::deltaTime = screenView->deltaTime;
}

void DrawUtils::setColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

Tessellator* DrawUtils::get3dTessellator() {
	//auto myTess = *reinterpret_cast<Tessellator**>(game3dContext + 0xB0);
	return *reinterpret_cast<Tessellator**>(game3dContext + 0xB0);
}

__int64 DrawUtils::getScreenContext() {
	return game3dContext == 0 ? screenContext2d : game3dContext;
}

MatrixStack* DrawUtils::getMatrixStack() {
	return reinterpret_cast<MatrixStack*>(*reinterpret_cast<__int64*>(DrawUtils::getScreenContext() + 0x18i64) + 0x30i64);
}

float DrawUtils::getTextWidth(std::string* textStr, float textSize) {
	TextHolder text(*textStr);

	return renderCtx->getLineLength(mcFont, &text, textSize, false);
}

float DrawUtils::getFontHeight(float textSize) {

	return mcFont->getLineHeight() * textSize;
}

void DrawUtils::flush() {
	renderCtx->flushText(0);
}

void DrawUtils::drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
	DrawUtils::tess__begin(tessellator, 3, 3);

	tess_vertex(tessellator, p1.x, p1.y, 0);
	tess_vertex(tessellator, p2.x, p2.y, 0);
	tess_vertex(tessellator, p3.x, p3.y, 0);

	meshHelper_renderImm(screenContext2d, tessellator, uiMaterial);
}


void DrawUtils::drawQuad(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4) {
	DrawUtils::tess__begin(tessellator, 1, 4);

	tess_vertex(tessellator, p1.x, p1.y, 0);
	tess_vertex(tessellator, p2.x, p2.y, 0);
	tess_vertex(tessellator, p3.x, p3.y, 0);
	tess_vertex(tessellator, p4.x, p4.y, 0);

	meshHelper_renderImm(screenContext2d, tessellator, uiMaterial);
}

void DrawUtils::drawLine(const Vec2& start, const Vec2& end, float lineWidth) {
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	DrawUtils::tess__begin(tessellator, 3, 6);

	tess_vertex(tessellator, start.x + modX, start.y + modY, 0);
	tess_vertex(tessellator, start.x - modX, start.y - modY, 0);
	tess_vertex(tessellator, end.x - modX, end.y - modY, 0);

	tess_vertex(tessellator, start.x + modX, start.y + modY, 0);
	tess_vertex(tessellator, end.x + modX, end.y + modY, 0);
	tess_vertex(tessellator, end.x - modX, end.y - modY, 0);

	meshHelper_renderImm(screenContext2d, tessellator, uiMaterial);
}

void DrawUtils::drawText(const Vec2& pos, std::string* textStr, const MC_Color& color, float textSize, float alpha, bool shadow) {
	TextHolder text(*textStr);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;

	float posF[4];  // Vec4(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x;
	posF[2] = pos.y;
	posF[3] = pos.y;

	TextMeasureData textMeasure{};
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;
	textMeasure.displayShadow = shadow;

	renderCtx->drawText(mcFont, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}

void DrawUtils::drawCenteredText(Vec2 pos, std::string text, float size, float textOpacity) {
	pos.x -= DrawUtils::getTextWidth(&text, size) / 2.f;
	pos.y -= DrawUtils::getFontHeight(size) / 2.f;
	DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, textOpacity);
}

void DrawUtils::drawBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool fill, int mode) {
	// Calculate the dimensions of the box
	Vec3 diff = upper.sub(lower);

	// Create an array of vertices representing the corners of the box
	Vec3 vertices[8];
	vertices[0] = Vec3(lower.x, lower.y, lower.z);
	vertices[1] = Vec3(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = Vec3(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = Vec3(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = Vec3(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = Vec3(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = Vec3(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = Vec3(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	if (fill) {
		// Convert the vertices to screen coordinates
		std::vector<Vec2> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2 screen;
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
		std::vector<std::tuple<int, Vec2>> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2 screen;
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
			std::tuple<int, Vec2> start = *it;
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
			Vec2 lastDir(0, -1);
			do {
				float smallestAngle = PI * 2;
				Vec2 smallestDir;
				std::tuple<int, Vec2> smallestE;
				auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
				for (auto cur : screenCords) {
					if (std::get<0>(current) == std::get<0>(cur))
						continue;

					// angle between vecs
					Vec2 dir = Vec2(std::get<1>(cur)).sub(std::get<1>(current));
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

			Vec2 lastVertex;
			bool hasLastVertex = false;
			for (auto& indice : indices) {
				Vec2 curVertex = std::get<1>(screenCords[indice]);
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

void DrawUtils::draw2DBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool fill, bool corners) {
	if (Game.getLocalPlayer() == nullptr) return;
	Vec3 worldPoints[8];
	worldPoints[0] = Vec3(lower.x, lower.y, lower.z);
	worldPoints[1] = Vec3(lower.x, lower.y, upper.z);
	worldPoints[2] = Vec3(upper.x, lower.y, lower.z);
	worldPoints[3] = Vec3(upper.x, lower.y, upper.z);
	worldPoints[4] = Vec3(lower.x, upper.y, lower.z);
	worldPoints[5] = Vec3(lower.x, upper.y, upper.z);
	worldPoints[6] = Vec3(upper.x, upper.y, lower.z);
	worldPoints[7] = Vec3(upper.x, upper.y, upper.z);

	std::vector<Vec2> points;
	for (int i = 0; i < 8; i++) {
		Vec2 result;
		if (refdef->OWorldToScreen(origin, worldPoints[i], result, fov, screenSize))
			points.emplace_back(result);
	}
	if (points.size() < 1) return;

	Vec4 resultRect = {points[0].x, points[0].y, points[0].x, points[0].y};
	for (const auto& point : points) {
		if (point.x < resultRect.x) resultRect.x = point.x;
		if (point.y < resultRect.y) resultRect.y = point.y;
		if (point.x > resultRect.z) resultRect.z = point.x;
		if (point.y > resultRect.w) resultRect.w = point.y;
	}
	if (fill) DrawUtils::fillRectangle(Vec2(resultRect.x, resultRect.y), Vec2(resultRect.z, resultRect.w));
	if (!corners)
		DrawUtils::drawRectangle(Vec2(resultRect.x, resultRect.y), Vec2(resultRect.z, resultRect.w), lineWidth);
	else {
		float length = (resultRect.x - resultRect.z) / 4.f;

		// Top left
		DrawUtils::drawLine(Vec2(resultRect.x, resultRect.y), Vec2(resultRect.x - length, resultRect.y), lineWidth);
		DrawUtils::drawLine(Vec2(resultRect.x, resultRect.y), Vec2(resultRect.x, resultRect.y - length), lineWidth);

		// Top right
		DrawUtils::drawLine(Vec2(resultRect.z, resultRect.y), Vec2(resultRect.z + length, resultRect.y), lineWidth);
		DrawUtils::drawLine(Vec2(resultRect.z, resultRect.y), Vec2(resultRect.z, resultRect.y - length), lineWidth);

		// Bottom left
		DrawUtils::drawLine(Vec2(resultRect.x, resultRect.w), Vec2(resultRect.x - length, resultRect.w), lineWidth);
		DrawUtils::drawLine(Vec2(resultRect.x, resultRect.w), Vec2(resultRect.x, resultRect.w + length), lineWidth);

		// Bottom right
		DrawUtils::drawLine(Vec2(resultRect.z, resultRect.w), Vec2(resultRect.z + length, resultRect.w), lineWidth);
		DrawUtils::drawLine(Vec2(resultRect.z, resultRect.w), Vec2(resultRect.z, resultRect.w + length), lineWidth);
	}
}

void DrawUtils::drawNameTags(Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	Vec2 textPos;
	Vec4 rectPos;
	std::string text = ent->getNameTag()->getText();
	text = Utils::sanitize(text);

	float textWidth = getTextWidth(&text, textSize);
	float textHeight = getFontHeight(textSize);
	static MC_Color blackColor(0, 0, 0);
	if (refdef->OWorldToScreen(origin, ent->eyePos0.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * textSize;
		rectPos.y = textPos.y - 1.f * textSize;
		rectPos.z = textPos.x + textWidth + 1.f * textSize;
		rectPos.w = textPos.y + textHeight + 2.f * textSize;
		Vec4 subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * textSize;
		static auto nametagsMod = moduleMgr->getModule<NameTags>();
		fillRectangle(rectPos, blackColor, nametagsMod->opacity);
		drawText(textPos, &text, MC_Color(255, 255, 255), textSize);

		static auto nameTagsMod = moduleMgr->getModule<NameTags>();

		if (ent->getEntityTypeId() == 319 && nameTagsMod->displayArmor) {  // is player, show armor
			auto* player = reinterpret_cast<Player*>(ent);
			float scale = textSize * 0.6f;
			float spacing = scale + 15.f;
			float x = rectPos.x + 1.f * textSize;
			float y = rectPos.y - 20.f * scale;
			// armor
			for (int i = 0; i < 4; i++) {
				ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					//DrawUtils::drawItem(stack, Vec2(x, y), 1.f, scale, stack->isEnchanted());
					DrawUtils::drawItem(stack, Vec2(x, y), scale);
					DrawUtils::drawBarItem(stack, Vec2(x, y), scale);
					x += scale * spacing;
				}
			}
			// item
			{
				ItemStack* stack = player->getSelectedItem();
				if (stack->item != nullptr) {
					//DrawUtils::drawItem(stack, Vec2(rectPos.z - 1.f - 15.f * scale, y), 1.f, scale, stack->isEnchanted());
					DrawUtils::drawItem(stack, Vec2(rectPos.z - 1.f - 15.f * scale, y), scale);
					DrawUtils::drawBarItem(stack, Vec2(rectPos.z - 1.f - 15.f * scale, y), scale);
				}
			}
		}
	}
}

void DrawUtils::drawEntityBox(Entity* ent, float lineWidth, bool fill) {
	Vec3 end = ent->eyePos0;
	AABB render;
	if (ent->isPlayer()) {
		render = AABB(end, ent->width, ent->height, ent->height);
		render.upper.y += 0.2f;
		render.lower.y += 0.2f;
	} else
		render = AABB(end, ent->width, ent->height, 0);
	render.upper.y += 0.1f;

	float LineWidth = (float)fmax(0.5f, 1 / (float)fmax(1, (float)Game.getLocalPlayer()->eyePos0.dist(end)));
	DrawUtils::drawBox(render.lower, render.upper, lineWidth == 0 ? LineWidth : lineWidth, fill);
}

void DrawUtils::draw2D(Entity* ent, float lineWidth) {
	if (Game.getLocalPlayer() == nullptr) return;
	Vec3 end = ent->eyePos0;
	AABB render;
	if (ent->isPlayer()) {
		render = AABB(end, ent->width, ent->height, ent->height);
		render.upper.y += 0.2f;
		render.lower.y += 0.2f;
	} else
		render = AABB(end, ent->width, ent->height, 0);
	render.upper.y += 0.1f;

	Vec3 worldPoints[8];
	worldPoints[0] = Vec3(render.lower.x, render.lower.y, render.lower.z);
	worldPoints[1] = Vec3(render.lower.x, render.lower.y, render.upper.z);
	worldPoints[2] = Vec3(render.upper.x, render.lower.y, render.lower.z);
	worldPoints[3] = Vec3(render.upper.x, render.lower.y, render.upper.z);
	worldPoints[4] = Vec3(render.lower.x, render.upper.y, render.lower.z);
	worldPoints[5] = Vec3(render.lower.x, render.upper.y, render.upper.z);
	worldPoints[6] = Vec3(render.upper.x, render.upper.y, render.lower.z);
	worldPoints[7] = Vec3(render.upper.x, render.upper.y, render.upper.z);

	std::vector<Vec2> points;
	for (int i = 0; i < 8; i++) {
		Vec2 result;
		if (refdef->OWorldToScreen(origin, worldPoints[i], result, fov, screenSize))
			points.emplace_back(result);
	}
	if (points.size() < 1) return;

	Vec4 resultRect = {points[0].x, points[0].y, points[0].x, points[0].y};
	for (const auto& point : points) {
		if (point.x < resultRect.x) resultRect.x = point.x;
		if (point.y < resultRect.y) resultRect.y = point.y;
		if (point.x > resultRect.z) resultRect.z = point.x;
		if (point.y > resultRect.w) resultRect.w = point.y;
	}
	float LineWidth = (float)fmax(0.5f, 1 / (float)fmax(1, (float)Game.getLocalPlayer()->eyePos0.dist(end)));
	DrawUtils::drawRectangle(Vec2(resultRect.x, resultRect.y), Vec2(resultRect.z, resultRect.w), lineWidth == 0 ? LineWidth : lineWidth);
}

void DrawUtils::drawItem(ItemStack* item, const Vec2& itemPos, float opacity, float scale, bool isEnchanted) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<ScreenContext*>(scnCtx);
	BaseActorRenderContext baseActorRenderCtx(screenCtx, Game.getClientInstance(), Game.getClientInstance()->minecraftGame);
	ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
}

void DrawUtils::drawItem(ItemStack* item, const Vec2& itemPos, float scale) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<ScreenContext*>(scnCtx);
	BaseActorRenderContext baseActorRenderCtx(screenCtx, Game.getClientInstance(), Game.getClientInstance()->minecraftGame);
	ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, 1.f, scale, false);
	if (item->isGlint()) {
		DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
		renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, 1.f, scale, true);
	}
}

void DrawUtils::drawBarItem(ItemStack* stack, const Vec2& ItemPos, float scale) {
	float dura1 = stack->getItem()->getMaxDamage();
	float dura2 = stack->getItem()->getDamageValue(stack->tag);
	float dura3 = dura1 - dura2;
	int dura4 = dura3 / dura1 * 100;
	if (dura3 != dura1) {
		MC_Color gamerColor = MC_Color(1.f - dura3 / dura1, dura3 / dura1, 0.f);
		Vec4 BarPos = Vec4(ItemPos.x + 1.5f * scale, ItemPos.y + 12.5f * scale, ItemPos.x + 15.f * scale, ItemPos.y + 14.5f * scale);
		Vec4 rect = Vec4(BarPos.x, BarPos.y, BarPos.x + ((BarPos.z - BarPos.x) / 100 * dura4), BarPos.w - 1.f * scale);
		DrawUtils::fillRectangle(BarPos, MC_Color(0, 0, 0), 1.f);
		DrawUtils::fillRectangle(rect, MC_Color(gamerColor), 1.f);
	}
}

Vec2 DrawUtils::worldToScreen(const Vec3& world) {
	Vec2 ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
void DrawUtils::drawLine3d(const Vec3& start, const Vec3& end, bool onUi) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 2);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	tess_vertex(myTess, start1.x, start1.y, start1.z);
	tess_vertex(myTess, end1.x, end1.y, end1.z);

	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);
}
void DrawUtils::drawBox3d(const Vec3& lower, const Vec3& upper, float scale, bool onUi) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 12);

	Vec3 diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	auto newLower = lower.sub(origin);

	Vec3 vertices[8];
	vertices[0] = Vec3(newLower.x, newLower.y, newLower.z);
	vertices[1] = Vec3(newLower.x + diff.x, newLower.y, newLower.z);
	vertices[2] = Vec3(newLower.x, newLower.y, newLower.z + diff.z);
	vertices[3] = Vec3(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

	vertices[4] = Vec3(newLower.x, newLower.y + diff.y, newLower.z);
	vertices[5] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
	vertices[6] = Vec3(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
	vertices[7] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);
	// Scale vertices using glm
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	Vec3 newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);  // .add(0.4375f, 0.4375f, 0.4375f) is chest
	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = Vec3{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
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

void DrawUtils::drawBox3dFilled(const Vec3& lower, const Vec3& upper, float scale, bool outline, bool onUi) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();
	Vec3 diff = upper.sub(lower);
	Vec3 newLower = lower.sub(origin);
	Vec3 vertices[8] = {
		{newLower.x, newLower.y, newLower.z},
		{newLower.x + diff.x, newLower.y, newLower.z},
		{newLower.x, newLower.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y, newLower.z + diff.z},

		{newLower.x, newLower.y + diff.y, newLower.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z},
		{newLower.x, newLower.y + diff.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z}};
	// Scale vertices using glm
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	Vec3 newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);  // .add(0.4375f, 0.4375f, 0.4375f) is chest
	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = Vec3{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
	}

	DrawUtils::tess__begin(myTess, 1);
	static int v[48] = {5, 7, 6, 4, 4, 6, 7, 5, 1, 3, 2, 0, 0, 2, 3, 1, 4, 5, 1, 0, 0, 1, 5, 4, 6, 7, 3, 2, 2, 3, 7, 6, 4, 6, 2, 0, 0, 2, 6, 4, 5, 7, 3, 1, 1, 3, 7, 5};
	for (int i = 0; i < 48; i++) tess_vertex(myTess, vertices[v[i]].x, vertices[v[i]].y, vertices[v[i]].z);
	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);

	if (!outline) return;
	DrawUtils::tess__begin(myTess, 4, 12);
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

void DrawUtils::drawCircle(Vec2 pos, Vec2 radius, MC_Color color, double quality) {
	DrawUtils::setColor(color.r, color.g, color.b, color.a);
	DrawUtils::tess__begin(tessellator, 5);

	for (int i = 0; i <= 360 / quality; i++) {
		double x2 = sin(((i * quality * PI) / 180)) * radius.x;
		double y2 = cos(((i * quality * PI) / 180)) * radius.y;
		tess_vertex(tessellator, pos.x + x2, pos.y + y2, 0);
	}
	meshHelper_renderImm(screenContext2d, tessellator, uiMaterial);
}

void DrawUtils::drawCircleFilled(Vec2 pos, Vec2 radius, MC_Color color, double quality) {
	float x;
	float y;
	DrawUtils::setColor(color.r, color.g, color.b, color.a);
	for (double i = 0; i <= 360;) {
		DrawUtils::tess__begin(tessellator, 3);
		x = radius.x * (float)cos(i);
		y = radius.y * (float)sin(i);
		tess_vertex(tessellator, x + pos.x, y + pos.y, 0);
		i = i + quality;
		x = radius.x * (float)cos(i);
		y = radius.y * (float)sin(i);
		tess_vertex(tessellator, x + pos.x, y + pos.y, 0);
		tess_vertex(tessellator, pos.x, pos.y, 0);
		i = i + quality;
	}

	meshHelper_renderImm(screenContext2d, tessellator, uiMaterial);
}

void DrawUtils::fillRectangle(const Vec4& pos, const MC_Color& col, float alpha) {
	DrawUtils::setColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}
inline void DrawUtils::tess__begin(Tessellator* tess, int vertexFormat, int numVerticesReserved) {
	__int64 tessellator = reinterpret_cast<__int64>(tess);

	using tess_begin_t = void(__fastcall*)(Tessellator*, int, int);
	static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(FindSignature("48 89 5C 24 ?? 55 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 45"));
	tess_begin(tess, vertexFormat, numVerticesReserved);
}
void DrawUtils::setGameRenderContext(__int64 ctx) {
	game3dContext = ctx;
	if (Game.getClientInstance()->levelRenderer != nullptr)
		origin = Game.getClientInstance()->levelRenderer->getOrigin();
}
Vec3 DrawUtils::getOrigin() {
	return origin;
}
void DrawUtils::drawLinestrip3d(const std::vector<Vec3>& points) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();

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

void DrawUtils::onMouseClickUpdate(int key, bool isDown) {
	if (Game.isInGame())
		switch (key) {
		case 1:  // Left Click
			isLeftClickDown = isDown;
			if (isDown)
				shouldToggleLeftClick = true;
			break;
		case 2:  // Right Click
			isRightClickDown = isDown;
			if (isDown)
				shouldToggleRightClick = true;
			break;
		}
}
