#pragma once

class Trail : public Module
{
public:
	Trail(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("Trail", "Render", "Draw Trail", keybind, enabled)
	{
		registerEnumSetting("Mode", "The type of mode", { "Ball" }, &mode);
		registerFloatSetting("Circle Size", "Change circle size", &size, 5, 30);
		registerBoolSetting("Gradient", "Change circle size", &gradient);
	}
	float size = 3;

	int delay = 12;
	int delayCount = 0;
	int timeLong = 1000;

	int mode = 0;
	bool gradient = false;

	struct TrailLog {
		int time;
		Vector3<float> pos;
		UIColor color;
	};
	long long timeOffset = 0;
	std::vector<TrailLog> trailLog;
	Vector3<float> oldPos;

	void onEvent(UpdateTickEvent* event) {
		switch (mode) {
		case 0: // Ball
			delayCount++;
			if (delayCount <= delay)return;
			delayCount = 0;
			if (timeOffset == 0) timeOffset = TimeUtil::getCurrentMs();;
			auto localPlayer = Game::GetLocalPlayer();
			if (localPlayer == nullptr) return;
			int time = TimeUtil::getCurrentMs() - timeOffset;
			auto pos = localPlayer->GetPosition();
			if (oldPos.distance(pos) > 0.01) {
				TrailLog trail = {};
				trail.time = time;
				trail.pos = localPlayer->GetPosition();
				if (gradient) {
					trail.color = ColorUtil::getClientColor(5, 1, 1, 1);
				}
				else {
					trail.color = UIColor(255, 255, 255);
				}
				trailLog.push_back(trail);
				oldPos = pos;
			}

			for (auto it = trailLog.begin(); it != trailLog.end(); ) {
				if ((time - it->time) > timeLong) {
					it = trailLog.erase(it);
				}
				else {
					++it;
				}
			}
			break;
		}
	}

	void onEvent(ImGUIRenderEvent* event) override
	{
		if (!Game::GetInstance()->mcGame->canUseMoveKeys())return;
		switch (mode) {
		case 0: // Ball
			int time = TimeUtil::getCurrentMs() - timeOffset;
			for (auto trail : trailLog) {
				int time2 = time - trail.time;
				int time3 = timeLong - time2;
				Vector2<float> screen, screen2, screen3;
				if (!Game::GetInstance()->WorldToScreen(trail.pos, screen)) continue;
				if (!Game::GetInstance()->WorldToScreen(Vector3<float>(trail.pos.x - 0.1f, trail.pos.y - 0.1f, trail.pos.z - 0.1f), screen2)) continue;
				if (!Game::GetInstance()->WorldToScreen(Vector3<float>(trail.pos.x + 0.1f, trail.pos.y + 0.1f, trail.pos.z + 0.1f), screen2)) continue;

				float circleSize = std::min(std::max(abs(screen.x - screen2.x), abs(screen.y - screen2.y)), size);
				float opacity = time3 < 300 ? time3 / 300.f : 1;
				circleSize = circleSize * (time2 < 100 ? time2 / 100.f : 1) * opacity;

				ImRenderUtil::fillCircle(screen, circleSize, trail.color, opacity, 12.f);
				if (circleSize <= 2) continue;
				ImRenderUtil::fillShadowCircle(screen, circleSize, trail.color, opacity, 40.f, 0);
			}
			break;
		}
	}

	void onEnabled() override {
		switch (mode) {
		case 0: // Ball
			timeOffset = TimeUtil::getCurrentMs();
			break;
		}
	}

	void onDisabled() override {
		switch (mode) {
		case 0: // Ball
			trailLog.clear();
			break;
		}
	}
};