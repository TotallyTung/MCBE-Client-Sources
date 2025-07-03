#pragma once

struct Color {
	union {
		struct { float r, g, b, a; };
		float arr[4]{};
	};

	Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}

	Color(const float r, const float g, const float b, const float a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	Color lerp(Color target, float pct) {
		return Color((this->r + (target.r - this->r) * pct) * 255, (this->g + (target.g - this->g) * pct) * 255, (this->b + (target.b - this->b) * pct) * 255, 255);
	}
};

std::unordered_map<char, Color> mColorMap = {
	{'0', Color(0, 0, 0)},        // Black
	{'1', Color(0, 0, 170)},      // Dark Blue
	{'2', Color(0, 170, 0)},      // Dark Green
	{'3', Color(0, 170, 170)},    // Dark Aqua
	{'4', Color(170, 0, 0)},      // Dark Red
	{'5', Color(170, 0, 170)},    // Dark Purple
	{'6', Color(255, 170, 0)},    // Gold
	{'7', Color(170, 170, 170)},  // Gray
	{'8', Color(85, 85, 85)},     // Dark Gray
	{'9', Color(85, 85, 255)},    // Blue
	{'a', Color(85, 255, 85)},    // Green
	{'b', Color(85, 255, 255)},   // Aqua
	{'c', Color(255, 85, 85)},    // Red
	{'d', Color(255, 85, 255)},   // Light Purple
	{'e', Color(255, 255, 85)},   // Yellow
	{'f', Color(255, 255, 255)},  // White
	{'r', Color(255, 255, 255)}   // Reset
};