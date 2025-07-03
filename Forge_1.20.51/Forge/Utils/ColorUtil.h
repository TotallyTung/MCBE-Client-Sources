#pragma once
#include <random>

UIColor r1;
UIColor g1;
UIColor b1;

UIColor r2;
UIColor g2;
UIColor b2;

UIColor rgb1;
UIColor rgb2;

bool rainbow;
bool wave;

//Color Stuff

class ColorUtil
{
public:
	inline static UIColor lerpColor(UIColor color1, UIColor color2, float amount)
	{
		amount = std::min(1.f, std::max(0.f, amount));
		return UIColor(Math::lerpInt(color1.r * 255, color2.r * 255, amount), Math::lerpInt(color1.g * 255, color2.g * 255, amount), Math::lerpInt(color1.b * 255, color2.b * 255, amount), Math::lerpInt(color1.a * 255, color2.a * 255, amount));
	}

	inline static UIColor lerpColors(int seconds, int index, UIColor start, UIColor end)
	{
		int angle = (int)(((TimeUtil::getCurrentMs()) / seconds + index) % 360);
		return lerpColor(start, end, ((angle >= 180 ? 360 - angle : angle) * 2) / 360.0f);
	}

	inline static UIColor RainbowDark(float seconds, float saturation, float brightness, int index)
	{
		float hue = ((TimeUtil::getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000);
		float r, g, b = 0;
		hsvToRgb(hue, saturation, brightness, r, g, b);
		return UIColor(r * 190, g * 190, b * 190);
	}

	inline static UIColor rainbowColor(float seconds, float saturation, float brightness, long index, float alpha) {
		float hue6 = (((TimeUtil::getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000));
		float r, g, b = 0;
		hsvToRgb(hue6, saturation, brightness, r, g, b);

		return UIColor(r * 255, g * 255, b * 255, alpha);
	}

	inline static UIColor waveColor(int red, int green, int blue, int red2, int green2, int blue2, long index, float alpha) {
		double offset = ((TimeUtil::getCurrentMs() - index) / 8) / (double)120;
		double aids123 = ((TimeUtil::getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((TimeUtil::getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(red * inverse_percent + red2 * aids123);
		int greenPart = (int)(green * inverse_percent + green2 * aids123);
		int bluePart = (int)(blue * inverse_percent + blue2 * aids123);

		return UIColor(redPart, greenPart, bluePart, alpha);
	}

	inline static UIColor astolfoRainbow(int yOffset, int yTotal, float alpha) {
		float speed = 3000;
		float hue = (float)(TimeUtil::getCurrentMs() % (int)speed) + ((yTotal - yOffset) * 9);
		while (hue > speed) { hue -= speed; } hue /= speed;
		if (hue > 0.5) { hue = 0.5F - (hue - 0.5f); } hue += 0.5f;
		float r, g, b = 0;
		hsvToRgb(hue, 0.5, 1.f, r, g, b);

		return UIColor(r * 255, g * 255, b * 255, alpha);
	}

	inline static UIColor ClientColor(int index) {
		return waveColor(2, 69, 95, 255, 255, 255, index, 255);
	}


private:
	inline static void hsvToRgb(float h, float s, float v, float& rOut, float& gOut, float& bOut)
	{

		if (s == 0.0f)
		{
			// gray
			rOut = gOut = bOut = v;
			return;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0:
			rOut = v;
			gOut = t;
			bOut = p;
			break;
		case 1:
			rOut = q;
			gOut = v;
			bOut = p;
			break;
		case 2:
			rOut = p;
			gOut = v;
			bOut = t;
			break;
		case 3:
			rOut = p;
			gOut = q;
			bOut = v;
			break;
		case 4:
			rOut = t;
			gOut = p;
			bOut = v;
			break;
		case 5:
		default:
			rOut = v;
			gOut = p;
			bOut = q;
			break;
		}
	}
};
