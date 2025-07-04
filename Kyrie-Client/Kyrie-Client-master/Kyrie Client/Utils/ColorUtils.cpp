#include "ColorUtils.h"

Color ColorUtils::lerp(const Color& startColor, const Color& endColor, const float& duration) {
	int R = (int)((endColor.r - startColor.r) * duration);
	int G = (int)((endColor.g - startColor.g) * duration);
	int B = (int)((endColor.b - startColor.b) * duration);
	int A = (int)((endColor.a - startColor.a) * duration);
	return Color(startColor.r + R, startColor.g + G, startColor.b + B, startColor.a + A);
}