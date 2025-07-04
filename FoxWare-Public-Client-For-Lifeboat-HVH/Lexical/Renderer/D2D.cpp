#include "D2D.h"
#include <unordered_map>
#include <winrt/base.h>
#include "../Client/Client.h"
#include "../SDK/Game.h"
#include "../Utils/TimerUtil.h"
#include "../Utils/Logger.h"

float D2D::deltaTime = 0.016f;

//d2d stuff
static ID2D1Factory3* d2dFactory = nullptr;
static IDWriteFactory* d2dWriteFactory = nullptr;
static ID2D1Device2* d2dDevice = nullptr;
static ID2D1DeviceContext2* d2dDeviceContext = nullptr;
static ID2D1Bitmap1* sourceBitmap = nullptr;
static ID2D1Effect* blurEffect = nullptr;

//cache
static std::unordered_map<float, winrt::com_ptr<IDWriteTextFormat>> textFormatCache;
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutCache;
static std::unordered_map<uint32_t, winrt::com_ptr<ID2D1SolidColorBrush>> colorBrushCache;

//temporary cache
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutTemporary;

static int currentD2DFontSize = 25;
static std::string currentD2DFont = "Arial";
static bool isFontItalic = false;

static bool initD2D = false;

template<typename T>
void SafeRelease(T*& ptr) {
	if (ptr != nullptr) {
		ptr->Release();
		ptr = nullptr;
	}
}

std::wstring to_wide(const std::string& str);
uint64_t getTextLayoutKey(const std::string& textStr, float textSize);
IDWriteTextFormat* getTextFormat(float textSize);
IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout = true);
ID2D1SolidColorBrush* getSolidColorBrush(const UIColor& color);

void D2D::NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi) {
	if (!initD2D) {

		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dFactory);

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(d2dWriteFactory), reinterpret_cast<IUnknown**>(&d2dWriteFactory));

		IDXGIDevice* dxgiDevice;
		d3d11Device->QueryInterface<IDXGIDevice>(&dxgiDevice);
		d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
		dxgiDevice->Release();

		d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
		//d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &d2dDeviceContext);

		d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

		IDXGISurface* dxgiBackBuffer = nullptr;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		D2D1_BITMAP_PROPERTIES1 bitmapProperties
			= D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), fxdpi, fxdpi);
		d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &sourceBitmap);
		dxgiBackBuffer->Release();
		d2dDeviceContext->SetTarget(sourceBitmap);

		initD2D = true;
	}

	d2dDeviceContext->BeginDraw();
}

void D2D::EndFrame() {
	if (!initD2D)
		return;

	d2dDeviceContext->EndDraw();

	static CustomFont* customFontMod = ModuleManager::getModule<CustomFont>();
	if ((currentD2DFont != customFontMod->getSelectedFont()) || (currentD2DFontSize != customFontMod->fontSize) || (isFontItalic != customFontMod->italic)) {
		currentD2DFont = customFontMod->getSelectedFont();
		currentD2DFontSize = customFontMod->fontSize;
		isFontItalic = customFontMod->italic;
		textFormatCache.clear();
		textLayoutCache.clear();
		//textLayoutTemporary.clear();
	}

	static float timeCounter = 0.0f;
	timeCounter += D2D::deltaTime;
	if (timeCounter > 90.f) {
		if (textFormatCache.size() > 1000)
			textFormatCache.clear();

		if (textLayoutCache.size() > 500)
			textLayoutCache.clear();

		if (colorBrushCache.size() > 2000)
			colorBrushCache.clear();

		timeCounter = 0.0f;
	}

	textLayoutTemporary.clear();
}

void D2D::Render() {

	// NameTags
	{
		// Prevents NamTags render override HUD and Watermark
		/*static NameTags* nameTagsMod = ModuleManager::getModule<NameTags>();
		if (nameTagsMod->isEnabled())
			nameTagsMod->Render();*/
	}

	ModuleManager::onD2DRender();
	NotificationManager::Render();

	/*
	// Debug
	{
		char textFormatDb[30];
		sprintf_s(textFormatDb, 30, "TextFormatCaches: %i", (int)(textFormatCache.size()));

		char textLayoutDb[30];
		sprintf_s(textLayoutDb, 30, "TextLayoutCaches: %i", (int)(textLayoutCache.size()));

		char colorBrushDb[30];
		sprintf_s(colorBrushDb, 30, "ColorBrushCaches: %i", (int)(colorBrushCache.size()));

		drawText(Vec2<float>(10.f, 75.f), std::string(textFormatDb), UIColor(255, 255, 255), 0.5f, false);
		drawText(Vec2<float>(10.f, 90.f), std::string(textLayoutDb), UIColor(255, 255, 255), 0.5f, false);
		drawText(Vec2<float>(10.f, 105.f), std::string(colorBrushDb), UIColor(255, 255, 255), 0.5f, false);
	}
	*/
	// ClickGUI
	{
		static ClickGUI* clickGuiMod = ModuleManager::getModule<ClickGUI>();
		if (clickGuiMod->isEnabled())
			clickGuiMod->Render();
	}

	// Eject
	{
		Vec2<float> windowSize = D2D::getWindowSize();
		static float holdTime = 0.f;
		static float holdAnim = 0.f;
		static float showDuration = 0.f;
		static float exitDuration = 0.f;
		static float exitVelocity = 0.f;

		if (showDuration > 0.1f) {
			static std::string text = "Hold Ctrl and L to eject";
			float textSize = 1.f * showDuration;
			float textPaddingX = 4.f * textSize;
			float textPaddingY = 1.f * textSize;
			float textWidth = getTextWidth(text, textSize);
			float textHeight = getTextHeight(text, textSize);

			Vec2<float> textPos = Vec2<float>((windowSize.x - textWidth) / 2.f, -30.f + (100.f * showDuration));
			Vec4<float> rectPos = Vec4<float>(textPos.x - textPaddingX, textPos.y - textPaddingY, textPos.x + textWidth + textPaddingX, textPos.y + textHeight + textPaddingY);
			Vec4<float> underlineRect = Vec4<float>(rectPos.x, rectPos.w, rectPos.z, rectPos.w + 2.f * textSize);
			Vec4<float> underlineAnim = Vec4<float>(underlineRect.x, underlineRect.y, underlineRect.x + (underlineRect.z - underlineRect.x) * holdAnim, underlineRect.w);

			fillRectangle(rectPos, UIColor(0, 0, 0, 125));
			fillRectangle(underlineRect, UIColor(0, 0, 0, 165));
			fillRectangle(underlineAnim, UIColor(255, 255, 255));
			drawText(textPos, text, UIColor(255, 255, 255), textSize);
		}

		if (Game::isKeyDown(VK_CONTROL) && Game::isKeyDown('L')) {
			holdTime += D2D::deltaTime / 2.f;
			if (holdTime > 1.f)
				holdTime = 1.f;
			exitDuration = 1.5f;
		}
		else {
			holdTime = 0.f;
			exitDuration -= D2D::deltaTime;
		}

		holdAnim += (holdTime - holdAnim) * (D2D::deltaTime * 10.f);
		if (holdAnim > 1.f)
			holdAnim = 1.f;
		if (holdAnim < 0.f)
			holdAnim = 0.f;

		if (exitDuration > 0.f) {
			showDuration += (1.f - showDuration) * (D2D::deltaTime * 8.f);
			exitVelocity = 0.f;
		}
		else {
			showDuration -= exitVelocity;
			exitVelocity += D2D::deltaTime / 4.f;
		}

		if (showDuration < 0.f)
			showDuration = 0.f;
		if (showDuration > 1.f)
			showDuration = 1.f;

		if (holdAnim > 0.99f)
			Client::shutdown();
	}
	
}

void D2D::Clean() {
	if (!initD2D)
		return;

	SafeRelease(d2dFactory);
	SafeRelease(d2dWriteFactory);
	SafeRelease(d2dDevice);
	SafeRelease(d2dDeviceContext);
	SafeRelease(sourceBitmap);
	SafeRelease(blurEffect);

	textFormatCache.clear();
	textLayoutCache.clear();
	colorBrushCache.clear();

	textLayoutTemporary.clear();

	initD2D = false;
}

void D2D::Flush() {
	d2dDeviceContext->Flush();
}

Vec2<float> D2D::getWindowSize() {
	D2D1_SIZE_U size = sourceBitmap->GetPixelSize();
	return Vec2<float>((float)size.width, (float)size.height);
}

void D2D::drawText(const Vec2<float>& textPos, const std::string& textStr, const UIColor& color, float textSize, bool storeTextLayout) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout);

	static CustomFont* customFontMod = ModuleManager::getModule<CustomFont>();
	if (customFontMod->shadow) {
		ID2D1SolidColorBrush* shadowColorBrush = getSolidColorBrush(UIColor(0, 0, 0, color.a));
		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x + 1.f, textPos.y + 1.f), textLayout, shadowColorBrush);
	}

	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x, textPos.y), textLayout, colorBrush);
}

float D2D::getTextWidth(const std::string& textStr, float textSize, bool storeTextLayout) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout);
	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	return textMetrics.widthIncludingTrailingWhitespace;
}

float D2D::getTextHeight(const std::string& textStr, float textSize, bool storeTextLayout) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout);
	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	return std::ceilf(textMetrics.height);
}

void D2D::drawLine(const Vec2<float>& startPos, const Vec2<float>& endPos, const UIColor& color, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawLine(D2D1::Point2F(startPos.x, startPos.y), D2D1::Point2F(endPos.x, endPos.y), colorBrush, width);
}

void D2D::drawRectangle(const Vec4<float>& rect, const UIColor& color, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), colorBrush, width);
}

void D2D::fillRectangle(const Vec4<float>& rect, const UIColor& color) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), colorBrush);
}

void D2D::drawCircle(const Vec2<float>& centerPos, const UIColor& color, float radius, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush, width);
}

void D2D::fillCircle(const Vec2<float>& centerPos, const UIColor& color, float radius) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush);
}

void D2D::addBlur(const Vec4<float>& rect, float strength, bool flush) {
	if (flush) {
		d2dDeviceContext->Flush();
	}
	ID2D1Bitmap* targetBitmap = nullptr;
	D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(sourceBitmap->GetPixelFormat());
	d2dDeviceContext->CreateBitmap(sourceBitmap->GetPixelSize(), props, &targetBitmap);
	D2D1_POINT_2U destPoint = D2D1::Point2U(0, 0);
	D2D1_SIZE_U size = sourceBitmap->GetPixelSize();
	D2D1_RECT_U Rect = D2D1::RectU(0, 0, size.width, size.height);
	targetBitmap->CopyFromBitmap(&destPoint, sourceBitmap, &Rect);

	D2D1_RECT_F screenRectF = D2D1::RectF(0.f, 0.f, (float)sourceBitmap->GetPixelSize().width, (float)sourceBitmap->GetPixelSize().height);
	D2D1_RECT_F clipRectD2D = D2D1::RectF(rect.x, rect.y, rect.z, rect.w);

	blurEffect->SetInput(0, targetBitmap);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, strength);

	ID2D1Image* outImage = nullptr;
	blurEffect->GetOutput(&outImage);

	ID2D1ImageBrush* outImageBrush = nullptr;
	D2D1_IMAGE_BRUSH_PROPERTIES outImage_props = D2D1::ImageBrushProperties(screenRectF);
	d2dDeviceContext->CreateImageBrush(
		outImage,
		outImage_props,
		&outImageBrush
	);

	ID2D1RectangleGeometry* clipRectGeo = nullptr;
	d2dFactory->CreateRectangleGeometry(clipRectD2D, &clipRectGeo);
	d2dDeviceContext->FillGeometry(clipRectGeo, outImageBrush);

	targetBitmap->Release();
	outImage->Release();
	outImageBrush->Release();
	clipRectGeo->Release();
}

void D2D::lookAt(float viewMatrix[4][4], const Vec3<float>& eye, const Vec3<float>& center, const Vec3<float>& up) {
	Vec3<float> f = center.sub(eye).normalize();
	Vec3<float> s = f.cross(up).normalize();
	Vec3<float> u = s.cross(f);

	viewMatrix[0][0] = s.x;
	viewMatrix[1][0] = s.y;
	viewMatrix[2][0] = s.z;
	viewMatrix[3][0] = -s.dot(eye);

	viewMatrix[0][1] = u.x;
	viewMatrix[1][1] = u.y;
	viewMatrix[2][1] = u.z;
	viewMatrix[3][1] = -u.dot(eye);

	viewMatrix[0][2] = -f.x;
	viewMatrix[1][2] = -f.y;
	viewMatrix[2][2] = -f.z;
	viewMatrix[3][2] = f.dot(eye);

	viewMatrix[0][3] = 0.f;
	viewMatrix[1][3] = 0.f;
	viewMatrix[2][3] = 0.f;
	viewMatrix[3][3] = 1.f;
}

void D2D::perspective(float projectionMatrix[4][4], float fov, float aspect, float zNear, float zFar) {
	float tanHalfFov = tanf(fov / 2.f);
	projectionMatrix[0][0] = 1.f / (aspect * tanHalfFov);
	projectionMatrix[1][0] = 0.f;
	projectionMatrix[2][0] = 0.f;
	projectionMatrix[3][0] = 0.f;

	projectionMatrix[0][1] = 0.f;
	projectionMatrix[1][1] = 1.f / tanHalfFov;
	projectionMatrix[2][1] = 0.f;
	projectionMatrix[3][1] = 0.f;

	projectionMatrix[0][2] = 0.f;
	projectionMatrix[1][2] = 0.f;
	projectionMatrix[2][2] = zFar / (zFar - zNear);
	projectionMatrix[3][2] = -(zFar * zNear) / (zFar - zNear);

	projectionMatrix[0][3] = 0.f;
	projectionMatrix[1][3] = 0.f;
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][3] = 0.f;
}
std::wstring to_wide(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

uint64_t getTextLayoutKey(const std::string& textStr, float textSize) {
	std::hash<std::string> textHash;
	std::hash<float> textSizeHash;
	uint64_t combinedHash = textHash(textStr) ^ textSizeHash(textSize);
	return combinedHash;
}

IDWriteTextFormat* getTextFormat(float textSize) {
	if (textFormatCache[textSize].get() == nullptr) {
		std::wstring fontNameWide = to_wide(currentD2DFont);
		const WCHAR* fontName = fontNameWide.c_str();
		d2dWriteFactory->CreateTextFormat(
			fontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			isFontItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,		//DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)currentD2DFontSize * textSize,
			L"en-us", //locale
			textFormatCache[textSize].put()
		);
	}

	return textFormatCache[textSize].get();
}

IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout) {

	std::wstring wideText = to_wide(textStr);
	const WCHAR* text = wideText.c_str();
	IDWriteTextFormat* textFormat = getTextFormat(textSize);
	uint64_t textLayoutKey = getTextLayoutKey(textStr, textSize);

	if (storeTextLayout) {
		if (textLayoutCache[textLayoutKey].get() == nullptr) {
			d2dWriteFactory->CreateTextLayout(
				text,
				(UINT32)wcslen(text),
				textFormat,
				FLT_MAX,
				0.f,
				textLayoutCache[textLayoutKey].put()
			);
		}
		return textLayoutCache[textLayoutKey].get();
	}
	else {
		if (textLayoutTemporary[textLayoutKey].get() == nullptr) {
			d2dWriteFactory->CreateTextLayout(
				text,
				(UINT32)wcslen(text),
				textFormat,
				FLT_MAX,
				0.f,
				textLayoutTemporary[textLayoutKey].put()
			);
		}
		return textLayoutTemporary[textLayoutKey].get();
	}
}

ID2D1SolidColorBrush* getSolidColorBrush(const UIColor& color) {
	uint32_t colorBrushKey = ColorUtil::ColorToUInt(color);
	if (colorBrushCache[colorBrushKey].get() == nullptr) {
		d2dDeviceContext->CreateSolidColorBrush(color.toD2D1Color(), colorBrushCache[colorBrushKey].put());
	}
	return colorBrushCache[colorBrushKey].get();
}