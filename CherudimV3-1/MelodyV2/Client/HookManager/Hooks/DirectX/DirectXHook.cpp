#include "DirectXHook.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>

#include "../../../../Libs/ImGui/imgui.h"
#include "../../../../Libs/ImGui/imgui_internal.h"
#include "../../../../Libs/ImGui/imgui_impl_dx11.h"
#include "../../../../Libs/ImGui/imgui_impl_dx12.h"
#include "../../../../Libs/ImGui/imgui_impl_win32.h"
#include "../../../../Libs/ImFX/imfx.h"
#include "../../../../Libs/Kiero/kiero.h"

#include "../../../../Libs/Fonts/Mojangles.h"
#include "../../../../Libs/Fonts/ProductSansRegular.h"
#include "../../../../Libs/Fonts/ProductSansLight.h"
//#include "../../../../Libs/Fonts/TahomaRegular.h"
//#include "../../../../Libs/Fonts/Verdana.h"
#include "../../../../Libs/Fonts/NotoSans.h"

#include "../../../../SDK/GameData.h"
#include "../../../Client.h"

#include "../../../../Utils/ImGuiUtils.h"

void DirectXHook::Render(ImDrawList* drawlist) {
	ImGuiUtils::setDrawList(drawlist);

	ImGuiIO& io = ImGui::GetIO();
	static CustomFont* cFontMod = (CustomFont*)client->moduleMgr->getModule("CustomFont");

	switch (cFontMod->fontType) {
	case 0: { // Mojangles
		io.FontDefault = io.Fonts->Fonts[0];
		ImGuiUtils::setFontSize(22.f);
		ImGuiUtils::setShadowOffset(2.f);
		break;
	}
	case 1: { // ProductSans
		io.FontDefault = io.Fonts->Fonts[1];
		ImGuiUtils::setFontSize(25.f);
		ImGuiUtils::setShadowOffset(1.f);
		break;
	}
	case 2: { // ProductSansLight
		io.FontDefault = io.Fonts->Fonts[2];
		ImGuiUtils::setFontSize(25.f);
		ImGuiUtils::setShadowOffset(1.f);
		break;
	}
	case 3: { // NotoSans
		io.FontDefault = io.Fonts->Fonts[3];
		ImGuiUtils::setFontSize(25.f);
		ImGuiUtils::setShadowOffset(2.f);
		break;
	}
	}

	client->moduleMgr->onImGuiRender(drawlist);
	static ClickGui* clickGuiMod = (ClickGui*)client->moduleMgr->getModule("ClickGui");
	clickGuiMod->render(drawlist);
	Notifications::Render(drawlist);
	//ImGui::Text("%i", mc.getLocalPlayer()->getLevel()->getRuntimeActorList().size());
	//ImGuiUtils::drawText(Vec2<float>(10.f, 6.f), "Melody", UIColor(255, 0, 0, 255), 2.f);
}

HRESULT DirectXHook::presentCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags) {

	ID3D12Device* d3d12Device = nullptr;
	if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
		return oPresent(pSwapChain, syncInterval, flags);
	}
	pSwapChainRestore = pSwapChain;
	return oPresent(pSwapChain, syncInterval, flags);
}

HRESULT DirectXHook::resizeBuffersHook(IDXGISwapChain* ppSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags) {
	if (ImGui::GetCurrentContext() != nullptr) ImFX::CleanupFX();
	return oResizeBuffers(ppSwapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

__int64 DirectXHook::gameBgfxCallback(__int64 a1, __int64 a2, __int64 a3, __int64 a4) {
	__int64 result = gameBgfxFunc(a1, a2, a3, a4);
	if (pSwapChainRestore == nullptr) return result;
	if (!client->isInitialized()) return result;
	if (mc.getClientInstance() == nullptr || mc.getClientInstance()->guiData == nullptr) return result;

	HWND window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
	ID3D11Device* d3d11Device = nullptr;

	if (SUCCEEDED(pSwapChainRestore->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		static ID3D11DeviceContext* ppContext = nullptr;
		static ID3D11Texture2D* pBackBuffer = nullptr;
		static IDXGISurface* dxgiBackBuffer = nullptr;
		static ID3D11RenderTargetView* mainRenderTargetView = nullptr;

		d3d11Device->GetImmediateContext(&ppContext);
		pSwapChainRestore->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pSwapChainRestore->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

		if (!initImgui) {
			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromMemoryTTF((void*)Mojangles, sizeof(Mojangles), 100.f); // 100.f
			io.Fonts->AddFontFromMemoryTTF((void*)ProductSansRegular, sizeof(ProductSansRegular), 50.f); // 100.f
			io.Fonts->AddFontFromMemoryTTF((void*)ProductSansLight, sizeof(ProductSansLight), 50.f); // 100.f
			io.Fonts->AddFontFromMemoryTTF((void*)NotoSans, sizeof(NotoSans), 50.f); // 100.f
			io.IniFilename = NULL;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			initImgui = true;
		}

		ImFX::NewFrame(d3d11Device, dxgiBackBuffer, GetDpiForWindow(window));
		pBackBuffer->Release();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(mc.getClientInstance()->guiData->windowSizeReal.toImVec2());

		ImGui::NewFrame();
		Render(ImGui::GetBackgroundDrawList());
		ImGui::EndFrame();
		ImGui::Render();

		//Finish ImGui draw
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(drawData);
		ImFX::EndFrame();

		ppContext->Flush();
		//dxgiBackBuffer->Release();
		ppContext->Release();
		if (mainRenderTargetView) mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	return result;
}

void DirectXHook::init() {
	bool isDx11 = false;
	if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success) {
		if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			logF("Failed to initialize kiero!");
			return;
		}
		isDx11 = true;
	}

	if (!isDx11) {
		if (kiero::bind(145, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			logF("[ResizeBuffersHook] Failed to hook!");
			return;
		}
		logF("[ResizeBuffersHook] Successfully hooked!");
		if (kiero::bind(140, (void**)&oPresent, presentCallback) != kiero::Status::Success) {
			logF("[PresentHook] Failed to hook!");
			return;
		}
		logF("[PresentHook] Successfully hooked!");
	}
	else {
		if (kiero::bind(13, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			logF("[ResizeBuffersHook] Failed to hook!");
			return;
		}
		logF("[ResizeBuffersHook] Successfully hooked!");
		if (kiero::bind(8, (void**)&oPresent, presentCallback) != kiero::Status::Success) {
			logF("[PresentHook] Failed to hook!");
			return;
		}
		logF("[PresentHook] Successfully hooked!");
	}

	MemoryUtils::CreateHook("GameBgfxHook", MemoryUtils::getBase() + 0x387B090, (void*)&DirectXHook::gameBgfxCallback, (void*)&gameBgfxFunc);
	// Open stringView -> search  Render wait: %7.4f [ms] to find this Func
}