#pragma once
#include <Windows.h>
#include "Math.h"

struct Color {
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	Color(int red, int green, int blue, int alpha = 255) {
		this->r = red;
		this->g = green;
		this->b = blue;
		this->a = alpha;
	}
};

class ColorUtils {
private:
	inline static unsigned __int64 getCurrentMs() {
		FILETIME f;
		GetSystemTimeAsFileTime(&f);
		(unsigned long long)f.dwHighDateTime;
		unsigned __int64 nano = ((unsigned __int64)f.dwHighDateTime << 32LL) + (unsigned __int64)f.dwLowDateTime;
		return (nano - 116444736000000000LL) / 10000;
	}
public:
	static inline void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
		if (s == 0.0f) {
			// gray
			out_r = out_g = out_b = v;
			return;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i) {
		case 0:
			out_r = v;
			out_g = t;
			out_b = p;
			break;
		case 1:
			out_r = q;
			out_g = v;
			out_b = p;
			break;
		case 2:
			out_r = p;
			out_g = v;
			out_b = t;
			break;
		case 3:
			out_r = p;
			out_g = q;
			out_b = v;
			break;
		case 4:
			out_r = t;
			out_g = p;
			out_b = v;
			break;
		case 5:
		default:
			out_r = v;
			out_g = p;
			out_b = q;
			break;
		}
	};
public:
	static Color lerp(const Color& startColor, const Color& endColor, const float& duration);
	static Color getRainbowColor(float seconds, float saturation, float brightness, long index) {
		float currentHue = (((getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000));
		float red, green, blue = 0.0f;
		ColorConvertHSVtoRGB(currentHue, saturation, brightness, red, green, blue);

		return Color((int)(red * 255.f), (int)(green * 255.f), (int)(blue * 255.f));
	}
	static Color getWaveColor(const int& red, const int& green, const int& blue, const int& red2, const int& green2, const int& blue2, long index) {
		double aids123 = ((getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(red * inverse_percent + red2 * aids123);
		int greenPart = (int)(green * inverse_percent + green2 * aids123);
		int bluePart = (int)(blue * inverse_percent + blue2 * aids123);

		return Color(redPart, greenPart, bluePart);
	}
};