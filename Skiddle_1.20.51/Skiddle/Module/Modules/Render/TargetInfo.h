#pragma once
#include <vector>
#include <regex>

class TargetInfo : public Module {
public:
	TargetInfo(int keybind = Keyboard::NONE, bool enabled = false)
		: Module("TargetInfo", "Render", "Display Target", keybind, enabled) {
		registerBoolSetting("Show TargetHUD", "Show HP(targethud)", &showTargetHUD);
		registerBoolSetting("Show HP", "Show HP(targethud)", &showHp);
		registerBoolSetting("Show Trail", "Show Trail", &showTrail);
		registerFloatSetting("Trail Count", "Count of Trails", &trailCount, 1.f, 30.f);
		registerFloatSetting("Trail Distance", "Trail Distance", &trailDistance, 0.5f, 4.f);
		registerFloatSetting("Trail Opacity", "Trail Opacity", &trailOpacity, 0.f, 255.f);
		registerBoolSetting("Trail PP", "Tral PP(Parallel Projection)", &trailPP);
		registerFloatSetting("Trail Size", "Trail Size", &trailSize, 0.1f, 1.f);

		ingameOnly = true; // cant use keybind unless on the toast screen
	}
	bool showTargetHUD = true;
	bool showTrail = true;
	bool showHp = true;
	float animation = 0;
	std::string targetName = "";
	float targetHealth = 20.f;
	float trailCount = 12;
	float trailDistance = 1;
	float trailOpacity = 255;
	float trailSize = 1;
	bool trailPP = false;
	Vector3<float> targetPos;
	bool attacked = false;
	Player* attackedPlayer;
	static void ImScaleStart()
	{
		scale_start_index = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
	}

	static inline int scale_start_index;

	static ImVec2 ImScaleCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

		const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		for (int i = scale_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	static void ImScaleEnd(float scaleX, float scaleY, ImVec2 center = ImScaleCenter())
	{
		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

		for (int i = scale_start_index; i < buf.Size; i++)
		{
			ImVec2 pos = buf[i].pos - center;
			pos.x *= scaleX;
			pos.y *= scaleY;
			buf[i].pos = pos + center;
		}
	}

	ParticleEngine particleMgr;

	void onEvent(AttackEvent* event) {
		attacked = true;
		attackedPlayer = event->Player;
	}

	void onEvent(ImGUIRenderEvent* event) {
		if (getModuleByName("clickgui")->isEnabled()) return;
		static EasingUtil inEase;
		(!Game::TargetLists::auraList.empty() && getModuleByName("aura")->isEnabled()) ? inEase.incrementPercentage(ImRenderUtil::getDeltaTime() * 12.f / 10)
			: inEase.decrementPercentage(ImRenderUtil::getDeltaTime() * 2 * 12.f / 10);
		float inScale = inEase.easeOutExpo();
		if (inEase.isPercentageMax()) inScale = 1;
		animation = Math::lerp(0, 1, inEase.easeOutExpo());
		if (animation < 0.0001f) {
			return;
		}
		if (showTargetHUD) {
			ImScaleStart();
			auto pos = Vector2<float>(ImRenderUtil::getScreenSize().x / 2 + 140, ImRenderUtil::getScreenSize().y / 2);
			auto rect = Vector4<float>(pos.x, pos.y, pos.x + 250, pos.y + 80);
			if (!Game::TargetLists::auraList.empty())
			{
				auto target = Game::TargetLists::auraList[0];
				if (target->IsBadPtr())return;
				if (attackedPlayer == target && attacked) {
					if (TimeUtil::hasTimeElapsed("targethud", 1000 / 1.8, true)) {
						particleMgr.addParticles(2, pos.x + 40, pos.y + 40, 10, 6, 1.f, 1.5f, 0.6f);
						// particleMgr.addParticles(2, pos.x + 40, pos.y + 40, 10, 6, 1.f, 1.5f, 0.4f);
						// particleMgr.addParticles(2, pos.x + 40, pos.y + 40, 10, 6, 1.f, 1.5f, 0.4f);
						// particleMgr.addParticles(2, pos.x + 40, pos.y + 40, 10, 6, 1.f, 1.5f, 0.4f);
						attacked = false;
					}

				}
				auto _targetName = std::regex_replace((std::string)target->getNameTag()->getText(), std::regex("(\uFF82\uFF67.)|(\\[.*\\]\\s?)"), "");
				targetName = "";
				for (char c : _targetName) {
					if (c == '\n') break;
					targetName += c;
				}
				if (showHp) {
					auto health = target->getAttribute(25769804032);
					if (health->IsBadPtr())return;
					targetHealth = health->currentValue;
				}
				else {
					targetHealth = 20.f;
				}
				if (target == nullptr) return;
				targetPos = target->GetPosition();
			}
			ImRenderUtil::fillRectangle(rect, UIColor(0, 0, 0), 0.6f, 15.f);
			ImRenderUtil::fillShadowRectangle(rect, UIColor(0, 0, 0), 0.8f, 30.f, 0, 23.f);
			{
				static std::vector<std::vector<std::vector<float>>> lines{
					{{0,0,3.5f,3.5f},{39,27,11}},{{3.5f,0,7.5f,3.5f},{36,25,11}},{{7.5f,0,11.f,3.5f},{39,26,13}},{{11.f,0,15.f,3.5f},{33,23,9}},{{15.f,0,18.5f,3.5f},{30,20,7}},{{18.5f,0,22.5f,3.5f},{32,22,8}},{{22.5f,0,26.f,3.5f},{36,25,11}},{{26.f,0,30.f,3.5f},{35,24,11}},{{0,3.5f,7.5f,7.5f},{35,24,11}},{{7.5f,3.5f,11.f,7.5f},{36,25,11}},{{11.f,3.5f,15.f,7.5f},{43,30,14}},{{15.f,3.5f,18.5f,7.5f},{55,35,15}},{{18.5f,3.5f,22.5f,7.5f},{53,35,17}},{{22.5f,3.5f,26.f,7.5f},{37,25,12}},{{26.f,3.5f,30.f,7.5f},{27,18,8}},{{0,7.5f,3.5f,11.f},{36,25,11}},{{3.5f,7.5f,7.5f,11.f},{152,114,90}},{{7.5f,7.5f,11.f,11.f},{152,114,90}},{{11.f,7.5f,15.f,11.f},{158,118,95}},{{15.f,7.5f,18.5f,11.f},{165,125,107}},{{18.5f,7.5f,22.5f,11.f},{158,116,95}},{{22.5f,7.5f,26.f,11.f},{143,98,75}},{{26.f,7.5f,30.f,11.f},{43,31,15}},{{0,11.f,3.5f,15.f},{142,104,85}},{{3.5f,11.f,7.5f,15.f},{150,110,91}},{{7.5f,11.f,11.f,15.f},{150,110,91}},{{11.f,11.f,15.f,15.f},{142,104,85}},{{15.f,11.f,18.5f,15.f},{130,95,77}},{{18.5f,11.f,22.5f,15.f},{156,114,95}},{{22.5f,11.f,26.f,15.f},{130,88,63}},{{26.f,11.f,30.f,15.f},{130,88,63}},{{0,15.f,3.5f,18.5f},{150,110,91}},{{3.5f,15.f,7.5f,18.5f},{213,213,213}},{{7.5f,15.f,11.f,18.5f},{68,51,114}},{{11.f,15.f,15.f,18.5f},{151,103,86}},{{15.f,15.f,18.5f,18.5f},{156,114,95}},{{18.5f,15.f,22.5f,18.5f},{68,51,114}},{{22.5f,15.f,26.f,18.5f},{213,213,213}},{{26.f,15.f,30.f,18.5f},{142,104,85}},{{0,18.5f,3.5f,22.5f},{130,83,58}},{{3.5f,18.5f,7.5f,22.5f},{149,103,82}},{{7.5f,18.5f,11.f,22.5f},{153,103,95}},{{11.f,18.5f,15.f,22.5f},{88,53,40}},{{15.f,18.5f,18.5f,22.5f},{88,53,40}},{{18.5f,18.5f,22.5f,22.5f},{159,113,90}},{{22.5f,18.5f,26.f,22.5f},{135,88,59}},{{26.f,18.5f,30.f,22.5f},{107,69,43}},{{0,22.5f,3.5f,26.f},{120,78,56}},{{3.5f,22.5f,7.5f,26.f},{125,79,53}},{{7.5f,22.5f,11.f,26.f},{99,55,44}},{{11.f,22.5f,15.f,26.f},{99,55,44}},{{15.f,22.5f,18.5f,26.f},{99,55,44}},{{18.5f,22.5f,22.5f,26.f},{99,55,44}},{{22.5f,22.5f,26.f,26.f},{119,78,52}},{{26.f,22.5f,30.f,26.f},{108,69,48}},{{0,26.f,3.5f,30.5f},{93,58,37}},{{3.5f,26.f,7.5f,30.5f},{91,56,35}},{{7.5f,26.f,11.f,30.5f},{108,69,48}},{{11.f,26.f,15.f,30.5f},{108,69,48}},{{15.f,26.f,18.5f,30.5f},{102,65,43}},{{18.5f,26.f,22.5f,30.5f},{109,71,49}},{{22.5f,26.f,26.f,30.5f},{109,71,49}},{{26.f,26.f,30.f,30.5f},{102,65,43}}
				};
				float scale = 60 / 8 / 3.5f;
				auto offset = Vector2<float>(pos.x + 10, pos.y + 10);
				int index = 0;
				for (auto line : lines)
				{
					auto rect = Vector4<float>(line[0][0] * scale + offset.x, line[0][1] * scale + offset.y, line[0][2] * scale + offset.x, line[0][3] * scale + offset.y);
					auto color = UIColor(line[1][0], line[1][1], line[1][2]);
					ImRenderUtil::fillRectangle(rect, color, 1.f);
					index++;
				}
			}
			ImRenderUtil::drawText(Vector2<float>(pos.x + 75, pos.y + 8), &targetName, UIColor(255, 255, 255), 1.4f, 1.f, true);
			ImRenderUtil::drawText(Vector2<float>(pos.x + 75.5f, pos.y + 8), &targetName, UIColor(255, 255, 255), 1.4f, 1.f, true);
			if (showHp) {
				std::ostringstream oss;
				oss << std::fixed << std::setprecision(2) << targetHealth;
				std::string healthStr = oss.str() + (std::string)"HP";
				static float healthEase = targetHealth / 20.f;
				healthEase = Math::animate(targetHealth / 20.f, healthEase, RenderUtil::getDeltaTime() * 15.f);
				ImRenderUtil::drawText(Vector2<float>(pos.x + 75, pos.y + 35), &healthStr, UIColor(255, 255, 255), 1.f, 1.f, true);
				auto rect2 = Vector4<float>(pos.x + 75, pos.y + 55, pos.x + 75 + (165 * healthEase), pos.y + 65);
				if (targetHealth > 0.01) {
					ImRenderUtil::fillShadowRectangle(rect2, UIColor(255, 255, 255), 1.f, 20.f, 0, 70.f);
					ImRenderUtil::fillRectangle(rect2, UIColor(255, 255, 255), 1.f, 50.f);
				}
			}
			else {
				std::string healthStr = (std::string)"20.0 HP";
				static float healthEase = targetHealth / 20.f;
				healthEase = Math::animate(targetHealth / 20.f, healthEase, RenderUtil::getDeltaTime() * 15.f);
				ImRenderUtil::drawText(Vector2<float>(pos.x + 75, pos.y + 35), &healthStr, UIColor(255, 255, 255), 1.f, 1.f, true);
				auto rect2 = Vector4<float>(pos.x + 75, pos.y + 55, pos.x + 75 + (165 * healthEase), pos.y + 65);
				if (targetHealth > 0.01) {
					ImRenderUtil::fillShadowRectangle(rect2, UIColor(255, 255, 255), 1.f, 20.f, 0, 70.f);
					ImRenderUtil::fillRectangle(rect2, UIColor(255, 255, 255), 1.f, 50.f);
				}
			}
			particleMgr.drawParticles();
			particleMgr.updateParticles(RenderUtil::getDeltaTime());
			ImScaleEnd(inScale, inScale, ImVec2(pos.x + 125, pos.y + 40));
		}

		if (showTrail) {
			for (int i = 0; i < floor(trailCount); i++)
			{
				float angle = (360 / floor(trailCount)) * i;
				float rad = angle * (PI / 180);
				Vector3<float> pos = Vector3<float>(
					targetPos.x + (cos(rad) * trailDistance),
					targetPos.y + 1.f,
					targetPos.z + (sin(rad) * trailDistance)
				);
				float circleSize;
				Vector2<float> screen;
				if (!Game::GetInstance()->WorldToScreen(pos, screen)) continue;
				if (!trailPP) {
					Vector2<float> screen2, screen3;
					if (!Game::GetInstance()->WorldToScreen(Vector3<float>(pos.x - 0.1f, pos.y - 0.1f, pos.z - 0.1f), screen2)) continue;
					if (!Game::GetInstance()->WorldToScreen(Vector3<float>(pos.x + 0.1f, pos.y + 0.1f, pos.z + 0.1f), screen2)) continue;
					circleSize = std::min(std::max(abs(screen.x - screen2.x), abs(screen.y - screen2.y)), 20.f);
				}
				else {
					circleSize = 15.f;
				}
				circleSize = circleSize * animation * trailSize;
				UIColor color = ColorUtil::getClientColor(5, 1, 1, i * 10);
				ImRenderUtil::fillCircle(screen, circleSize, color, (trailOpacity / 255) * animation, 12.f);
				if (circleSize <= 2) continue;
				ImRenderUtil::fillShadowCircle(screen, circleSize, color, (trailOpacity / 255) * animation, 40.f, 0);
			}
		}
	}

	void onEnabled() override {}

private:
};