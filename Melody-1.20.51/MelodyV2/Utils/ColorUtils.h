// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include <Windows.h>
#include "../Libs/ImGui/imgui.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
struct MC_Color{
	union {
		struct {
			float r, g, b, a;
		};
		float arr[4];
	};
	bool shouldDelete = true;
	bool isFloat = true;
	MC_Color() {
		this->r = 1;
		this->g = 1;
		this->b = 1;
		this->a = 1;
	};

	MC_Color(const MC_Color& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		this->shouldDelete = other.shouldDelete;
	}

	MC_Color(const float* arr) {
		this->arr[0] = arr[0];
		this->arr[1] = arr[1];
		this->arr[2] = arr[2];
		this->arr[3] = arr[3];
	};

	MC_Color(const float r, const float g, const float b, const float a = 1) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	};

	MC_Color(const int r, const int g, const int b, const int a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	MC_Color(const bool isFloat, const float r, const float g, const float b, const float a = 255) {
		this->isFloat = isFloat;
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	MC_Color(const float r, const float g, const float b, const float a, const bool shouldDelete) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->shouldDelete = shouldDelete;
	};
};

struct UIColor {
	union
	{
		struct {
			int r, g, b, a;
		};
		int arr[4];
	};
	UIColor(int red = 255, int green = 255, int blue = 255, int alpha = 255) {
		this->r = red;
		this->g = green;
		this->b = blue;
		this->a = alpha;
	}
	static UIColor HSVtoRGB(float h, float s, float v) {
		float r, g, b;
		int i = int(h * 6);
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		return UIColor(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255), 255);
	}
	ImU32 toImU32() const {
		return IM_COL32(r, g, b, a);
	}
	MC_Color toMC_Color() const {  // Add 'const' here
		return MC_Color((float)this->r / 255.f, (float)this->g / 255.f, (float)this->b / 255.f, (float)this->a / 255.f);
	}
	MC_Color toMC_Color() {
		return MC_Color((float)this->r / 255.f, (float)this->g / 255.f, (float)this->b / 255.f, (float)this->a / 255.f);
	}
	ImColor toImColor() {
		return ImColor(this->r, this->g, this->b, this->a);
	}
};

class ColorUtils {
private:
	static inline float ImFabs(float x) { return fabsf(x); }
	template <typename T>
	static inline void ImSwap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}
	inline static unsigned __int64 getCurrentMs() {
		FILETIME f;
		GetSystemTimeAsFileTime(&f);
		(unsigned long long)f.dwHighDateTime;
		unsigned __int64 nano = ((unsigned __int64)f.dwHighDateTime << 32LL) + (unsigned __int64)f.dwLowDateTime;
		return (nano - 116444736000000000LL) / 10000;
	}
public:
	static std::string RGBtoHex(UIColor color) { // chatGPT on Top
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << std::hex << color.r;
		ss << std::setfill('0') << std::setw(2) << std::hex << color.g;
		ss << std::setfill('0') << std::setw(2) << std::hex << color.b;
		return ss.str();
	}
	static inline void RGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
		float K = 0.f;
		if (g < b) {
			ImSwap(g, b);
			K = -1.f;
		}
		if (r < g) {
			ImSwap(r, g);
			K = -2.f / 6.f - K;
		}

		const float chroma = r - (g < b ? g : b);
		out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
		out_s = chroma / (r + 1e-20f);
		out_v = r;
	};
	static UIColor HexToRGB(const std::string& hex) {
		UIColor color;
		std::stringstream ss;
		ss << std::hex << hex;

		unsigned int colorValue;
		ss >> colorValue;

		color.r = (colorValue >> 16) & 0xFF;
		color.g = (colorValue >> 8) & 0xFF;
		color.b = colorValue & 0xFF;
		return color;
	}

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
	static UIColor lerp(const UIColor& startColor, const UIColor& endColor, float duration) {
		int R = (int)((endColor.r - startColor.r) * duration);
		int G = (int)((endColor.g - startColor.g) * duration);
		int B = (int)((endColor.b - startColor.b) * duration);
		int A = (int)((endColor.a - startColor.a) * duration);
		return UIColor(startColor.r + R, startColor.g + G, startColor.b + B, startColor.a + A);
	}
	static UIColor getWaveColor(const UIColor& startColor, const UIColor& endColor, int index) {
		double offset = ((getCurrentMs() - index) / 8) / (double)120;
		double aids123 = ((getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(startColor.r * inverse_percent + endColor.r * aids123);
		int greenPart = (int)(startColor.g * inverse_percent + endColor.g * aids123);
		int bluePart = (int)(startColor.b * inverse_percent + endColor.b * aids123);
		return UIColor(redPart, greenPart, bluePart);
	}

	static UIColor getRainbowColor(float seconds, float saturation, float brightness, long index) {
		float currentHue = (((getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000));
		float red, green, blue = 0.0f;
		ColorConvertHSVtoRGB(currentHue, saturation, brightness, red, green, blue);

		return UIColor((int)(red * 255.f), (int)(green * 255.f), (int)(blue * 255.f));
	}
};