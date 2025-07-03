#pragma once
#include <random>
#include "Client.h"
static std::random_device rnd;
static std::mt19937 gen(rnd());
static std::uniform_real_distribution<double> dist(0., 1.);

class SplashScreen {
private:
	struct Triangle {
		int spawnMs;
		Vec2<float> pos;
		float size;
		bool filled;
	};
	static inline std::vector<Triangle> triangles;
	static inline int lastSpawnMs;
	static double RNG() {
		return dist(gen);
	}
public:
	static inline long long startMs;
	static inline bool showSplash = false;
	static inline bool showFlash = false;
	static int getCurrentMs() {
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}
	static void RenderFlash() {
		int ms = getCurrentMs() - startMs;
		if (ms > 1000) showFlash = false;
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = io.DisplaySize;
		drawList->AddRectFilled(ImVec2(0, 0), ImVec2(windowSize.x, windowSize.y), ImColor(255, 255, 255, (int)(std::max(0, 1000 - ms) / 1000.f * 255)));
	}

	static void Render() {
		int ms = (getCurrentMs() - startMs);
		if (ms > 3000) {
			showSplash = false;
			showFlash = true;
			startMs = getCurrentMs();
		}
		ImFont* font = ImGui::GetIO().Fonts->Fonts[2];
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = io.DisplaySize;
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		drawList->AddRectFilled(ImVec2(0, 0), ImVec2(windowSize.x, windowSize.y), ImColor(0, 0, 0));

		std::string str;
		float spacing = 0;
		auto fontSize = 4.f;
		if (ms >= 200)
			str = "wel";
		if (ms >= 400)
			str = "welcome";
		if (ms >= 700)
			str = "welcome to";
		if (ms >= 900) {
			str = "welcome to triangle!";
			spacing = std::min(ms - 900.f, 550.f) / 60.f;
		}
		if (ms >= 1060 && ms < 1450) {
			if (true) {
				int maxWidth = windowSize.x * 0.4f;
				int maxHeight = windowSize.x * 0.16f;
				Triangle triangle{
					ms,
					Vec2<float>(
						(windowSize.x / 2 - maxWidth / 2) + (RNG() * maxWidth),
						(windowSize.y / 2 - maxHeight / 2) + (RNG() * maxHeight)
					),
					(RNG() + 0.2f) * 80,
					rand() % 2 == 0
				};
				triangles.push_back(triangle);
				lastSpawnMs = ms;
			}
			for (Triangle triangle : triangles) {
				int opacity = (int)((1.f - (std::min(ms - triangle.spawnMs, 200) / 200.f)) * 255);
				ImVec2 p1 = ImVec2(triangle.pos.x - triangle.size / 2, triangle.pos.y + triangle.size / 2);
				ImVec2 p2 = ImVec2(triangle.pos.x, triangle.pos.y - triangle.size / 2);
				ImVec2 p3 = ImVec2(triangle.pos.x + triangle.size / 2, triangle.pos.y + triangle.size / 2);
				if (triangle.filled)
					drawList->AddTriangleFilled(p1, p2, p3, ImColor(255, 255, 255, opacity));
				else
					drawList->AddTriangle(p1, p2, p3, ImColor(255, 255, 255, opacity), 2.f);
			}
		}
		else
			triangles.clear();
		if (ms >= 1450 && ms <= 2080) {
			str = "ABCDE";
			auto iconFont = ImGui::GetIO().Fonts->Fonts[3];
			auto margin = 300;
			auto iconSize = 40.f;
			if (ms >= 1650) {
				margin = 250;
				iconSize += 50.f;
			}
			if (ms < 1850) {
				iconSize -= (ms - 1450.f) / 70.f;
			}
			else {
				margin = 130;
				iconSize += 50.f;
				iconSize += (ms - 1850.f) / 50.f;
			}
			auto textWidth = iconFont->CalcTextSizeA(iconSize, FLT_MAX, -1, str.c_str()).x + margin * str.length();
			textWidth -= margin;
			auto posX = windowSize.x / 2 - textWidth / 2;
			for (auto c : str) {
				auto str = (std::string)"" + c;
				auto measure = iconFont->CalcTextSizeA(iconSize, FLT_MAX, -1, str.c_str());
				auto pos = ImVec2(
					posX - measure.x / 2,
					windowSize.y / 2 - measure.y / 2
				);
				drawList->AddText(iconFont, iconSize, pos, ImColor(255, 255, 255), str.c_str());
				posX += measure.x + margin;
			}
			return;
		}
		if (ms >= 2080) {
			float scale = 1.f - std::pow((ms - 2080) / 920.f, 4.f);
			ImVec2 c = ImVec2(windowSize.x / 2, windowSize.y / 2);
			//drawList->AddImage((void*)Game::RenderInfo::kairunTexture, ImVec2(c.x - scale * 400, c.y - scale * 400), ImVec2(c.x + scale * 400, c.y + scale * 400));
			return;
		}

		auto textWidth = ImGuiUtils::getTextWidth(str, fontSize) + str.length() * spacing;
		auto textHeight = ImGuiUtils::getTextHeight(fontSize);
		{
			float posX = windowSize.x / 2 - textWidth / 2;
			for (auto c : str) {
				auto c2 = (std::string)"" + c;
				ImGuiUtils::drawText(Vec2<float>(posX, windowSize.y / 2 - textHeight / 2), c2, UIColor(255, 255, 255), fontSize, 1.f);
				posX += ImGuiUtils::getTextWidth(c2, fontSize) + spacing;
			}
		}
	};

};