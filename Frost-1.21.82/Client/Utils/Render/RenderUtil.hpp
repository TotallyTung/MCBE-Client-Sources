#pragma once

#include "../../../Memory/SDK/Render/MinecraftUIRenderContext.h"
#include "../../../Memory/SDK/Render/Context/ScreenContext.h"

enum Type
{
	Internal = 0, // UserPackage
	External = 1, // Raw
};

class RenderUtils
{
public:
	static inline MaterialPtr* uiMaterial = nullptr;
	static inline float* colorHolder;

	inline static void initialize() {
		colorHolder = InstanceManager::get<MinecraftUIRenderContext>()->mScreenContext->getColorHolder();

		if (uiMaterial == nullptr)
			 uiMaterial = MaterialPtr::createMaterial(HashedString("ui_fill_color"));

	}

	static void setColor(float r, float g, float b, float a) {
		if (colorHolder)
		{
			colorHolder[0] = r;
			colorHolder[1] = g;
			colorHolder[2] = b;
			colorHolder[3] = a;
			*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
		}
	}

	static void drawQuad(Vector2<float> p1, Vector2<float> p2, Vector2<float> p3, Vector2<float> p4) {
		if (!InstanceManager::get<Tessellator>() || !uiMaterial) return;

		Tessellator* tess = InstanceManager::get<Tessellator>();

		tess->begin(VertextFormat::QUAD);

		tess->vertex(p1.x, p1.y, 0);
		tess->vertex(p2.x, p2.y, 0);
		tess->vertex(p3.x, p3.y, 0);
		tess->vertex(p4.x, p4.y, 0);

		tess->renderMeshImmediately(InstanceManager::get<MinecraftUIRenderContext>()->mScreenContext, uiMaterial);
	}

	inline static void fillRectangle(Vector4<float> pos, Color col, float alpha) {
		setColor(col.r, col.g, col.b, alpha);
		drawQuad({ pos.x, pos.w }, { pos.z, pos.w }, { pos.z, pos.y }, { pos.x, pos.y });
	}

	static inline void fillRectangle(const Vector2<float>& start, const Vector2<float>& end) {
		drawQuad({ start.x, end.y }, { end.x, end.y }, { end.x, start.y }, { start.x, start.y });
	}

	inline static void drawRectangle(Vector4<float> pos, Color col, float alpha, float width) {
		InstanceManager::get<MinecraftUIRenderContext>()->drawRectangle(Vector4<float>(pos.x, pos.z, pos.y, pos.w), col, alpha, width);
	}

	static inline void drawRectangle(const Vector2<float>& start, const Vector2<float>& end, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRectangle({ start.x - lineWidth, start.y - lineWidth }, { end.x + lineWidth, start.y + lineWidth });  // TOP
		fillRectangle({ start.x - lineWidth, start.y }, { start.x + lineWidth, end.y });                          // LEFT
		fillRectangle({ end.x - lineWidth, start.y }, { end.x + lineWidth, end.y });                              //
		fillRectangle({ start.x - lineWidth, end.y - lineWidth }, { end.x + lineWidth, end.y + lineWidth });
	}

	static void drawLine(Vector2<int> start, Vector2<int> end, float lineWidth, Color col = Color(255, 255, 255), float alpha = 1.f) {
		setColor(col.r, col.g, col.b, alpha);

		Tessellator* tess = InstanceManager::get<Tessellator>();

		float modX = 0 - (start.y - end.y);
		float modY = start.x - end.x;

		float len = sqrtf(modX * modX + modY * modY);

		modX /= len;
		modY /= len;
		modX *= lineWidth;
		modY *= lineWidth;

		tess->begin(VertextFormat::TRIANGLE_STRIP);

		tess->vertex(start.x + modX, start.y + modY, 0);
		tess->vertex(start.x - modX, start.y - modY, 0);
		tess->vertex(end.x - modX, end.y - modY, 0);

		tess->vertex(start.x + modX, start.y + modY, 0);
		tess->vertex(end.x + modX, end.y + modY, 0);
		tess->vertex(end.x - modX, end.y - modY, 0);

		tess->renderMeshImmediately(InstanceManager::get<MinecraftUIRenderContext>()->mScreenContext, uiMaterial);
	}

	static void draw2D(Actor* ent, float lineWidth) 
	{
		if (InstanceManager::getLocalPlayer() == nullptr) return;
		Vector3<float> end = ent->getRenderPosition();
		AABB render;

		Vector3<float> lower = ent->getAABBShapeComponent()->mPosLower;
		Vector3<float> upper = ent->getAABBShapeComponent()->mPosUpper;

		Vector3<float> worldPoints[8];
		worldPoints[0] = Vector3<float>(lower.x, lower.y, lower.z);
		worldPoints[1] = Vector3<float>(lower.x, lower.y, upper.z);
		worldPoints[2] = Vector3<float>(upper.x, lower.y, lower.z);
		worldPoints[3] = Vector3<float>(upper.x, lower.y, upper.z);
		worldPoints[4] = Vector3<float>(lower.x, upper.y, lower.z);
		worldPoints[5] = Vector3<float>(lower.x, upper.y, upper.z);
		worldPoints[6] = Vector3<float>(upper.x, upper.y, lower.z);
		worldPoints[7] = Vector3<float>(upper.x, upper.y, upper.z);

		std::vector<Vector2<float>> points;
		for (int i = 0; i < 8; i++) {
			Vector2<float> result;
			if (InstanceManager::get<ClientInstance>()->WorldToScreen(worldPoints[i], result, FrameUtil::mTransform.mFov, FrameUtil::mTransform.mOrigin, FrameUtil::mTransform.mMatrix))
				points.emplace_back(result);
		}
		if (points.size() < 1) return;

		Vector4<float> resultRect = { points[0].x, points[0].y, points[0].x, points[0].y };
		for (const auto& point : points) {
			if (point.x < resultRect.x) resultRect.x = point.x;
			if (point.y < resultRect.y) resultRect.y = point.y;
			if (point.x > resultRect.z) resultRect.z = point.x;
			if (point.y > resultRect.w) resultRect.w = point.y;
		}
		float LineWidth = (float)fmax(0.5f, 1 / (float)fmax(1, (float)InstanceManager::getLocalPlayer()->getRenderPosition().distance(end)));
		drawRectangle(Vector2<float>(resultRect.x, resultRect.y), Vector2<float>(resultRect.z, resultRect.w), lineWidth == 0 ? LineWidth : lineWidth);
	}


	static float getTextWidth(std::string* str, float size)
	{
		return InstanceManager::get<MinecraftUIRenderContext>()->getLineLength(InstanceManager::get<Font>(), str, size, true);
	}

	static void drawText(Vector2<float> pos, std::string* str, Color color, float size, float alpha, bool shadow)
	{
		static CaretMeasureData black = CaretMeasureData(20, false);

		float tPos[4] = { pos.x, pos.x + 1000, pos.y, pos.y + 1000 };
		TextMeasureData data = TextMeasureData(size, shadow);

		InstanceManager::get<MinecraftUIRenderContext>()->drawText(InstanceManager::get<Font>(), tPos, str, color.arr, alpha, 0, &data, &black);
	}

	static void renderImage(std::string filePath, Vector2<float> ImagePos, Vector2<float> ImageDimension, Type type = External, Color color = Color(255, 255, 255), float alpha = 1.f)
	{
		mce::TexturePtr* texturePtr = new mce::TexturePtr();
		InstanceManager::get<MinecraftUIRenderContext>()->getTexture(texturePtr, new ResourceLocation(type, filePath));
		InstanceManager::get<MinecraftUIRenderContext>()->drawImage(texturePtr->mClientTexture.get(), &ImagePos, &ImageDimension, Vector2<float>(0.f, 0.f), Vector2<float>(1.f, 1.f));
		flushImage(color, alpha);
	}

	static void renderImage(std::string filePath, Vector4<float> ImagePos, Type type = External, Color color = Color(255, 255, 255), float alpha = 1.f)
	{
		renderImage(filePath, Vector2<float>(ImagePos.x, ImagePos.y), Vector2<float>(ImagePos.z, ImagePos.w), type, color, alpha);
	}

	static void flushImage(Color color = Color(255, 255, 255), float alpha = 1.f) // Later TODO remind me
	{
		static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
		InstanceManager::get<MinecraftUIRenderContext>()->flushImages(color, alpha, flushString);
	}
};