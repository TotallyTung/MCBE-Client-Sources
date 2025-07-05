#pragma once

#include "D2D.h"
#include <Prax/Utils/System/Logger.h>

#include <d2d1_3.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dwrite.h>
#include <dxgi1_4.h>
#include <imgui.h>


static winrt::com_ptr<ID2D1Factory3> d2dFactory = nullptr;
static winrt::com_ptr<ID2D1Device> d2dDevice = nullptr;

static winrt::com_ptr<ID2D1Effect> blurEffect = nullptr;
static winrt::com_ptr<ID2D1Bitmap1> sourceBitmap = nullptr;

static winrt::com_ptr<ID2D1DeviceContext> d2dDeviceContext = nullptr;
static winrt::com_ptr<ID2D1SolidColorBrush> brush = nullptr;

wchar_t font[32]               = L"Segoe UI";
winrt::com_ptr<IDWriteFactory> writeFactory  = nullptr;
winrt::com_ptr<IDWriteTextFormat> textFormat = nullptr;

bool initD2D = false;

float dpi = 0.0f;

void CHK(HRESULT hr) {
    if (FAILED(hr))
        throw std::runtime_error("HRESULT is failed value.");
}

void D2D::Init(IDXGISwapChain* pSwapChain, ID3D11Device* pDevice) {

    if (initD2D) return;

    if (initD2D == false && SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.put()))) {
        Logger::Write("DirectX", "Writing Factory created.", Logger::Debug);
        winrt::com_ptr<IDXGIDevice> dxgiDevice;
        pDevice->QueryInterface<IDXGIDevice>(dxgiDevice.put());
        if (dxgiDevice == nullptr) {
            return;
        }
        Logger::Write("DirectX", "Device QueryInterface", Logger::Debug);

        d2dFactory->CreateDevice(dxgiDevice.get(), d2dDevice.put());
        if (d2dDevice == nullptr) {
            return;
        }
        Logger::Write("DirectX", "Device created.", Logger::Debug);

        d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2dDeviceContext.put());
        if (d2dDeviceContext == nullptr) {
            return;
        }
        Logger::Write("DirectX", "Device Context created.", Logger::Debug);

        //Create blur
        d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, blurEffect.put());
        if (blurEffect == nullptr) {
            return;
        }
        Logger::Write("DirectX", "Blur effect created", Logger::Debug);

    }
    Logger::Write("DirectX", "Initialized D2D.", Logger::Debug);

    initD2D = true;
}

void D2D::BeginRender(IDXGISurface* surface, float fxdpi) {
    D2D1_BITMAP_PROPERTIES1 bitmapProperties
        = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                                  D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    d2dDeviceContext->CreateBitmapFromDxgiSurface(surface, &bitmapProperties, sourceBitmap.put());

}

void D2D::Shutdown() {
    try {
        if (!initD2D) return;

        d2dDeviceContext->Flush();

        d2dFactory = nullptr;
        d2dDevice = nullptr;
        blurEffect = nullptr;
        sourceBitmap = nullptr;
        d2dDeviceContext = nullptr;
        brush = nullptr;
        writeFactory = nullptr;
        textFormat = nullptr;

        initD2D = false;
    }
    catch (...) {
        Logger::Write("DirectX", "Error while shutting down D2D!", Logger::Error);
    }

}

void D2D::EndRender() {
    sourceBitmap = nullptr;
}

void D2D::DrawFilledRect(ImVec4 rect, D2D1::ColorF color) {
    d2dDeviceContext->CreateSolidColorBrush(color, brush.put());
    d2dDeviceContext->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), brush.get());
    brush->Release();
}


struct BlurCallbackData {
    float strength = 0.0f;
    float rounding = 0.0f;
    std::optional<ImVec4> clipRect;

    BlurCallbackData(float strength, float rounding, std::optional<ImVec4> clipRect = std::optional<ImVec4>()) {
        this->strength = strength;
        this->rounding = rounding;
        this->clipRect = clipRect;
    }
};

void D2D::BlurCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
    //Do D2D rendering stuff

    if (!initD2D) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    BlurCallbackData* data = (BlurCallbackData*)cmd->UserCallbackData;
    ImVec4 clipRect = data->clipRect.has_value() ? *data->clipRect : cmd->ClipRect;

    d2dDeviceContext->SetTarget(sourceBitmap.get());
    d2dDeviceContext->BeginDraw();

    //Copy the bitmap
    ID2D1Bitmap* targetBitmap = nullptr;
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(sourceBitmap->GetPixelFormat());
    d2dDeviceContext->CreateBitmap(sourceBitmap->GetPixelSize(), props, &targetBitmap);
    auto destPoint = D2D1::Point2U(0, 0);
    auto size = sourceBitmap->GetPixelSize();
    auto rect = D2D1::RectU(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    targetBitmap->CopyFromBitmap(&destPoint, sourceBitmap.get(), &rect);

    //Create rects and rounded rects
    D2D1_RECT_F screenRectF = D2D1::RectF(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    D2D1_RECT_F clipRectD2D = D2D1::RectF(
            clipRect.x,
            clipRect.y,
            clipRect.z,
            clipRect.w
    );
    D2D1_ROUNDED_RECT clipRectRounded = D2D1::RoundedRect(clipRectD2D, data->rounding, data->rounding);

    //Apply blur effect
    blurEffect->SetInput(0, targetBitmap);
    blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, data->strength);
    blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
    blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

    //Get the output image
    ID2D1Image* outImage = nullptr;
    blurEffect->GetOutput(&outImage);

    //Create bitmap brush for the clipping
    ID2D1ImageBrush* outImageBrush = nullptr;
    D2D1_IMAGE_BRUSH_PROPERTIES outImage_props = D2D1::ImageBrushProperties(screenRectF);
    d2dDeviceContext->CreateImageBrush(
            outImage,
            outImage_props,
            &outImageBrush
    );

    //Draw the blur
    ID2D1RoundedRectangleGeometry* clipRectGeo = nullptr;
    d2dFactory->CreateRoundedRectangleGeometry(clipRectRounded, &clipRectGeo);
    d2dDeviceContext->FillGeometry(clipRectGeo, outImageBrush);
    clipRectGeo->Release();

    //Finish Direct2d draw
    d2dDeviceContext->Flush();
    d2dDeviceContext->EndDraw();
    //Release interfaces
    outImageBrush->Release();
    outImage->Release();
    targetBitmap->Release();
    //Free the callback data
    //delete data;
}


bool D2D::AddBlur(ImDrawList* drawList, float strength, std::optional<ImVec4> clipRect, float rounding)
{
    if (!initD2D) {
        return false;
    }

    if (strength == 0)
        return false;

    BlurCallbackData* data = new BlurCallbackData(strength, rounding, clipRect);
    drawList->AddCallback(BlurCallback, data);
    return true;
}
