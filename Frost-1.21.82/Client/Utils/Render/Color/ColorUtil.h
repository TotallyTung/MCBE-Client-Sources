#pragma once

class ColorUtil
{
public:
	inline static Color lerpColor(Color color1, Color color2, float amount)
	{
		amount = std::min(1.f, std::max(0.f, amount));
		return Color(MathUtil::lerpInt(color1.r * 255, color2.r * 255, amount), MathUtil::lerpInt(color1.g * 255, color2.g * 255, amount), MathUtil::lerpInt(color1.b * 255, color2.b * 255, amount), MathUtil::lerpInt(color1.a * 255, color2.a * 255, amount));
	}

	inline static Color Rainbow(float seconds, float saturation, float brightness, int index) {
		float hue = ((TimeUtil::getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000);
		float r, g, b = 0;
		hsvToRgb(hue, saturation, brightness, r, g, b);
		return Color(r * 255, g * 255, b * 255);
	}

	inline static Color getAstolfoRainbow(float seconds, float saturation, float brightness, int index)
	{
		float hue = 0;
		float speed = seconds * 1450.f;
		float tempHue = (float)((TimeUtil::getCurrentMs() + index) % (int)speed);
		while (tempHue > speed)
			tempHue -= speed;
		tempHue /= speed;
		if (tempHue > 0.5)
			tempHue = 0.5F - (tempHue - 0.5f);
		tempHue += 0.5F;
		hue = tempHue;


		float r, g, b = 0;
		hsvToRgb(hue, saturation, brightness, r, g, b);
		return Color(r * 240, g * 240, b * 240);
	}

	inline static Color getCustomColors(float seconds, int index, const std::vector<std::pair<float, Color>>& colorMap)
	{
		float hue = ((TimeUtil::getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000);

		Color currentColor = Color(0, 0, 0);
		for (int i = 0; i < colorMap.size() - 1; i++)
		{
			if (hue >= colorMap[i].first && hue <= colorMap[i + 1].first)
			{
				float amount = (hue - colorMap[i].first) / (colorMap[i + 1].first - colorMap[i].first);
				currentColor = lerpColor(colorMap[i].second, colorMap[i + 1].second, amount);
				break;
			}
		}
		return Color(currentColor.r * 255, currentColor.g * 255, currentColor.b * 255);
	}

	inline static Color getClientColor(float seconds, float saturation, float brightness, int index) {
		std::vector<std::pair<float, Color>> colorMap;

		switch (GlobalVariables::mClientColor)
		{
		case 0: // Frost
			colorMap =
			{
				{0.0f, GlobalVariables::mClientMainColor},
				{0.5f, GlobalVariables::mClientSecondColor},
				{1.0f, GlobalVariables::mClientMainColor}
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 1: // Blue and Green
			colorMap =
			{
				{0.0f, Color(65, 201, 163)},
				{0.5f, Color(0, 13, 175)},
				{1.0f, Color(65, 201, 163)}
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 2: // Astolofo
			return getAstolfoRainbow(seconds, saturation * 0.5f, brightness, index * 2);
			break;
		case 3: // Bubble Gum
			colorMap =
			{
				{0.0f, Color(255, 192, 203)},
				{0.2f, Color(255, 153, 204)},
				{0.4f, Color(255, 102, 178)},
				{0.6f, Color(255, 51, 180)},
				{0.8f, Color(255, 0, 128)},
				{1.0f, Color(255, 192, 203)}
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 4: // Rainbow
			return Rainbow(seconds, saturation, brightness, index * 2);
			break;
		case 5: // Tenacity
		{
			colorMap =
			{
				{0.0f, Color(236.f, 133.f, 209.f)},
				{0.5f, Color(28.f, 167.f, 222.f)},
				{1.0f, Color(236.f, 133.f, 209.f)}
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 6: // Wave
			colorMap =
			{
				{0.0f, Color(33, 33, 33)},
				{0.5f, Color(255, 255, 255)},
				{1.0f, Color(33, 33, 33)}
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 7: // Cherry
			colorMap =
			{
				{0.0f, Color(139, 0, 0)},
				{0.25f, Color(205, 92, 92)},
				{0.5f, Color(220, 20, 60)},
				{0.75f, Color(255, 36, 0)},
				{1.0f, Color(139, 0, 0)},
			};
			return getCustomColors(seconds, index * 2, colorMap);
			break;
		case 8: // Xextreame
			colorMap =
			{
				{0.0f, Color(255, 255, 255)},
				{0.5f, Color(226, 13, 15)},
				{1.0f, Color(255, 255, 255)}
			};
			return getCustomColors(seconds, index, colorMap);
			break;
		case 9: // White
			return Color(255, 255, 255);
			break;
		}
		};
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
