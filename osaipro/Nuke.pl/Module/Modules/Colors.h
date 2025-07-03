#pragma once
#include "Module.h"
#include "../../Utils/ColorUtil.h"
#include "../../Utils/TimerUtil.h"
#include "../ModuleManager.h"
class Colors : public IModule {
public:
	Colors();
	SettingEnum color = this;
	float speed = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	int seperation = 25;
	int spacing = 70;
	float r = 255.f, g = 255.f, b = 255.f;
	float r2 = 32.f, g2 = 32.f, b2 = 32.f;
	MC_Color rainbowColor(float seconds, float saturation, float brightness, long index) {
		float hue6 = (((TimerUtil::getCurrentMs() + index) % (int)(6000)) / (float)(6000));
		float r, g, b = 0;
		Utils::ColorConvertHSVtoRGB(hue6, saturation, brightness, r, g, b);

		return MC_Color(r * 255, g * 255, b * 255);
	}

	MC_Color RGBWave(int red, int green, int blue, int red2, int green2, int blue2, long index) {
		float hue = ((TimerUtil::getCurrentMs()) % (int)(((int)10) * 1000)) / (float)(((int)10) * 1000);
		float r, g, b = 0;
		Utils::ColorConvertHSVtoRGB(hue, saturation, 1, r, g, b);
		double offset = ((TimerUtil::getCurrentMs() - index) / 2) / (double)120;
		double aids123 = ((TimerUtil::getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((TimerUtil::getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(r * red * inverse_percent + r * red2 * aids123);
		int greenPart = (int)(g * green * inverse_percent + g * green2 * aids123);
		int bluePart = (int)(b * blue * inverse_percent + b * blue2 * aids123);

		return MC_Color(redPart, greenPart, bluePart);
	}

	MC_Color waveColor(int red, int green, int blue, int red2, int green2, int blue2, long index) {
		double offset = ((TimerUtil::getCurrentMs() - index) / 8) / (double)120;
		double aids123 = ((TimerUtil::getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((TimerUtil::getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(red * inverse_percent + red2 * aids123);
		int greenPart = (int)(green * inverse_percent + green2 * aids123);
		int bluePart = (int)(blue * inverse_percent + blue2 * aids123);

		return MC_Color(redPart, greenPart, bluePart);
	}

	MC_Color astolfoRainbow(int yOffset, int yTotal) {
		float speed = 3000;
		float hue = (float)(TimerUtil::getCurrentMs() % (int)speed) + ((yTotal - yOffset) * 9);
		while (hue > speed) { hue -= speed; } hue /= speed;
		if (hue > 0.5) { hue = 0.5F - (hue - 0.5f); } hue += 0.5f;
		float r, g, b = 0;
		Utils::ColorConvertHSVtoRGB(hue, 0.5, 1.f, r, g, b);

		return MC_Color(r * 255, g * 255, b * 255);
	}
	const UIColor getColor(int index = 0) {
		MC_Color color_ = rainbowColor(8, 1.F, 1.F, 1);
		switch (color.getSelectedValue()) {
		case 0: color_ = rainbowColor(speed, saturation, 1.F, -index * 4.f); break; /* Rainbow */
		case 1: color_ = astolfoRainbow(1 / 5, 1000); break; /* Astolfo */
		case 2: color_ = MC_Color(color_.r * 255, color_.g * 255, color_.b * 255); break; /* Wave */
		case 3: color_ = MC_Color(color_.r * 255, color_.g * 255, color_.b * 255); break; /* RGB Wave */
		}
		return UIColor((int)color_.r, (int)color_.g, (int)color_.b);
	}
	const MC_Color getColorV2(int index = 0) {
		UIColor NowCC = getColor(index);
		return MC_Color(NowCC.r, NowCC.g, NowCC.b);
	}
	virtual const char* getModuleName() override;
	virtual bool isEnabled() override;
	virtual bool isVisible() override;
	virtual void onImGuiRender(ImDrawList* d);
};