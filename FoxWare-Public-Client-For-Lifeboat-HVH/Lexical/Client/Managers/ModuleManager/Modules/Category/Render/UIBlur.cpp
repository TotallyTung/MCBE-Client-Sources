#include "UIBlur.h"
float blurH;
UIColor mainColorH = (0, 0, 0, 0);
UIBlur::UIBlur() : Module("ScreenFX", "weird stuff", Category::RENDER) {
	registerSetting(new ColorSetting("Color", "NULL", &mainColorH, UIColor(0, 0, 0, 0)));
	registerSetting(new SliderSetting<float>("Blur", "Background blur intensity", &blurH, 4.f, 0.f, 20.f));
}

void UIBlur::onD2DRender() {
	Vec2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;
	D2D::addBlur(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), blurH);
	D2D::fillRectangle(Vec4<float>(0.f, 0.f, screenSize.x, screenSize.y), mainColorH);
}