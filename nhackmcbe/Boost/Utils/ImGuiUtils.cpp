#include "ImGuiUtils.h"
#include "../Client/Client.h"

void ImGuiUtils::setDrawList(ImDrawList* d) {
	drawlist = d;
}

void ImGuiUtils::drawText(Vec2<float> textPos, std::string textStr, UIColor color, float textSize, bool shadow) {
	static CustomFont* cFontMod = (CustomFont*)client->moduleMgr->getModule("CustomFont");
	if (cFontMod->fontShadow && shadow) drawlist->AddText(nullptr, fontSize * textSize, textPos.add(shadowOffset * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), textStr.c_str());
	drawlist->AddText(nullptr, fontSize * textSize, textPos.toImVec2(), color.toImColor(), textStr.c_str());
}

float ImGuiUtils::getTextWidth(std::string textStr, float textSize) {
	return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, textStr.c_str()).x;
}

float ImGuiUtils::getTextHeight(float textSize) {
	return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, "").y;
}

void ImGuiUtils::drawRectangle(const Vec4<float>& rectPos, UIColor color, float lineWidth, float rounding, ImDrawFlags flags) {
	drawlist->AddRect(ImVec2(rectPos.x, rectPos.y), ImVec2(rectPos.z, rectPos.w), color.toImColor(), rounding, flags, lineWidth);
}

void ImGuiUtils::drawRectangle(Vec2<float> pMin, Vec2<float> pMax, UIColor color, float lineWidth, float rounding, ImDrawFlags flags) {
	drawlist->AddRect(pMax.toImVec2(), pMax.toImVec2(), color.toImColor(), rounding, flags, lineWidth);
}

void ImGuiUtils::fillRectangle(Vec4<float> rectPos, UIColor color, float rounding, ImDrawFlags flags) {
	drawlist->AddRectFilled(ImVec2(rectPos.x, rectPos.y), ImVec2(rectPos.z, rectPos.w), color.toImColor(), rounding, flags);
}

void ImGuiUtils::fillRectangle(Vec2<float> pMin, Vec2<float> pMax, UIColor color, float rounding, ImDrawFlags flags) {
	drawlist->AddRectFilled(pMin.toImVec2(), pMax.toImVec2(), color.toImColor(), rounding, flags);
}

void ImGuiUtils::drawBox(const AABB& blockAABB, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline) {
	Vec3<float> lower = blockAABB.lower;
	Vec3<float> upper = blockAABB.upper;

	Vec3<float> diff = upper.sub(lower);
	Vec3<float> vertices[8];
	vertices[0] = Vec3<float>(lower.x, lower.y, lower.z);
	vertices[1] = Vec3<float>(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = Vec3<float>(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = Vec3<float>(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = Vec3<float>(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = Vec3<float>(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = Vec3<float>(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = Vec3<float>(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	std::shared_ptr<glmatrixf> refdef = std::shared_ptr<glmatrixf>(mc.getClientInstance()->getbadrefdef()->correct());
	Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
	Vec2<float> fov = mc.getClientInstance()->getFov();
	Vec2<float> screenSize = mc.getClientInstance()->guiData->windowSizeReal;

	if (fill) {
		// Convert the vertices to screen coordinates
		std::vector<Vec2<float>> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2<float> screen;
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
			drawlist->AddQuadFilled(screenCords[std::get<0>(face)].toImVec2(), screenCords[std::get<1>(face)].toImVec2(), screenCords[std::get<2>(face)].toImVec2(), screenCords[std::get<3>(face)].toImVec2(), color.toImColor());
			//drawlist->AddQuadFilled(screenCords[std::get<3>(face)].toImVec2(), screenCords[std::get<2>(face)].toImVec2(), screenCords[std::get<1>(face)].toImVec2(), screenCords[std::get<0>(face)].toImVec2(), color.toImColor());
		}
	}

	{
		// Convert the vertices to screen coordinates
		std::vector<std::tuple<int, Vec2<float>>> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2<float> screen;
			if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
				screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
			}
		}

		// Return if there are less than two points to draw lines between
		if (screenCords.size() < 2) return;

		switch (outline) {
		case false: {
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

					if (shouldDraw) drawlist->AddLine(std::get<1>(from).toImVec2(), std::get<1>(to).toImVec2(), lineColor.toImColor(), lineWidth);
				}
			}
			return;
			break;
		}
		case true: {
			// Find start vertex
			auto it = screenCords.begin();
			std::tuple<int, Vec2<float>> start = *it;
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
			Vec2<float> lastDir(0, -1);
			do {
				float smallestAngle = PI * 2;
				Vec2<float> smallestDir;
				std::tuple<int, Vec2<float>> smallestE;
				auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
				for (auto cur : screenCords) {
					if (std::get<0>(current) == std::get<0>(cur))
						continue;

					// angle between vecs
					Vec2<float> dir = Vec2<float>(std::get<1>(cur)).sub(std::get<1>(current));
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

			Vec2<float> lastVertex;
			bool hasLastVertex = false;
			for (auto& indice : indices) {
				Vec2<float> curVertex = std::get<1>(screenCords[indice]);
				if (!hasLastVertex) {
					hasLastVertex = true;
					lastVertex = curVertex;
					continue;
				}

				drawlist->AddLine(lastVertex.toImVec2(), curVertex.toImVec2(), lineColor.toImColor(), lineWidth);
				lastVertex = curVertex;
			}
			return;
			break;
		}
		}
	}
}

void ImGuiUtils::drawBox(const Vec3<float>& blockPos, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline) {
	AABB blockAABB;
	blockAABB.lower = blockPos;
	blockAABB.upper.x = blockPos.x + 1.f;
	blockAABB.upper.y = blockPos.y + 1.f;
	blockAABB.upper.z = blockPos.z + 1.f;
	ImGuiUtils::drawBox(blockAABB, color, lineColor, lineWidth, fill, outline);
}

bool ImGuiUtils::worldToScreen(const Vec3<float>& pos, Vec2<float>& out) {
	std::shared_ptr<glmatrixf> refdef = std::shared_ptr<glmatrixf>(mc.getClientInstance()->getbadrefdef()->correct());
	Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos2;
	Vec2<float> fov = mc.getClientInstance()->getFov();
	Vec2<float> screenSize = mc.getClientInstance()->guiData->windowSizeReal;
	return refdef->OWorldToScreen(origin, pos, out, fov, screenSize);
}