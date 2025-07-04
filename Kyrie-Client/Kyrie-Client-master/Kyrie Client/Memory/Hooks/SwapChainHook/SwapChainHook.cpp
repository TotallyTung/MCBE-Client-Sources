#include "SwapChainHook.h"

#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_3.h>

#include "../../../Kiero/Kiero.h"

#include "../../../ImGui/imgui.h"
#include "../../../ImGui/imgui_internal.h"
#include "../../../ImGui/imgui_impl_dx11.h"
#include "../../../ImGui/imgui_impl_dx12.h"
#include "../../../ImGui/imgui_impl_win32.h"

#include "../../../Fonts/Arial.h"
#include "../../../Fonts/Mojangles.h"

#include "../../../Utils/Utils.h"

#include "../../../Client/Client.h"
#include "../../GameData/GameData.h"

#include "../../../Utils/RenderUtils.h"
#include "../../../ImFX/imfx.h"

bool initImgui = false;

typedef HRESULT(__thiscall* Present)(IDXGISwapChain3*, UINT, UINT);
Present oPresent;

void Render(ImDrawList* d) {
	if (ImGui::Begin("Test")) {
		//static auto VTable = *(uintptr_t**)mc.getLocalPlayer();
		//ImGui::Text("%llx", VTable[89]);
		ImGui::Text("%llx", mc.getLocalPlayer());
	}
	ImGui::End();
	//ImGui::ShowDemoWindow();

	if (mc.getClientInstance()->levelRenderer != nullptr) {
		glmatrixf* badrefdef = mc.getClientInstance()->getBadRefDef();
		std::shared_ptr<glmatrixf> refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
		client->moduleMgr->onImGui3DRender(d, refdef.get());
	}

	client->moduleMgr->onImGuiRender(d);
	static Colors* colorsMod = client->moduleMgr->getModule<Colors>();
	{ // Watermark
		static Vec2<float> waterMarkPos = Vec2<float>(10.f, 7.5f);
		RenderUtils::drawText(d, Vec2<float>(waterMarkPos.x, waterMarkPos.y), "K", colorsMod->getColor(), 1.5f, true, 75.f); // 75.f
		RenderUtils::drawText(d, Vec2<float>(waterMarkPos.x + RenderUtils::getTextWidth("K", 1.5f), waterMarkPos.y), "yrie", Color(255, 255, 255), 1.5f, true, 75.f); // 75.f
	}
	
	static ClickGui* clickGuiMod = client->moduleMgr->getModule<ClickGui>();
	clickGuiMod->Render(d);

}

HRESULT presentCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags) {
	if (!client->isInitialized() || mc.getClientInstance() == nullptr || mc.getClientInstance()->guiData == nullptr) return oPresent(pSwapChain, syncInterval, flags);

	HWND window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
	ID3D11Device* d3d11Device = nullptr;
	ID3D12Device* d3d12Device = nullptr;

	if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		ID3D11DeviceContext* ppContext = nullptr;
		ID3D11Texture2D* pBackBuffer = nullptr;
		IDXGISurface* dxgiBackBuffer = nullptr;
		ID3D11RenderTargetView* mainRenderTargetView = nullptr;

		d3d11Device->GetImmediateContext(&ppContext);
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

		if (!initImgui) {
			if (ImGui::GetCurrentContext() == nullptr) {
				ImGui::CreateContext();
			}

			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsDark();
			io.Fonts->AddFontFromMemoryTTF((void*)Arial, sizeof(Arial), 50.f);
			io.Fonts->AddFontFromMemoryTTF((void*)Mojangles, sizeof(Mojangles), 100.f);
			io.IniFilename = NULL;

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			initImgui = true;
		}

		ImFX::NewFrame(d3d11Device, dxgiBackBuffer, GetDpiForWindow(window));
		pBackBuffer->Release();
		//ImGuiContext* g = ImGui::GetCurrentContext();
		//g->HoveredIdAllowOverlap = true;
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(ImVec2(mc.getClientInstance()->guiData->windowSizeReal.x, mc.getClientInstance()->guiData->windowSizeReal.y));

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
	else if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
	}

	return oPresent(pSwapChain, syncInterval, flags);
}

typedef HRESULT(__thiscall* ResizeBuffers)(IDXGISwapChain*, int, int, int, DXGI_FORMAT, int);
ResizeBuffers oResizeBuffers;

HRESULT resizeBuffersHook(IDXGISwapChain* ppSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags) {

	ImFX::CleanupFX();
	return oResizeBuffers(ppSwapChain, bufferCount, width, height, newFormat, swapChainFlags);

};

void SwapChainHook::init() {
	bool isDx11 = false;
	if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success) {
		if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			Utils::DebugF("Failed to initialize kiero!");
			return;
		}
		isDx11 = true;
	}

	if (!isDx11) {
		if (kiero::bind(145, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			Utils::DebugF("Failed to hook Resize Buffers Hook!");
			return;
		}
		Utils::DebugF("Successfully hooked Resize Buffers Hook!");
		if (kiero::bind(140, (void**)&oPresent, presentCallback) != kiero::Status::Success) {
			Utils::DebugF("Failed to hook Present D3D12 Hook!");
			return;
		}
		Utils::DebugF("Successfully hooked Present D3D12 Hook!");
	}
	else {
		if (kiero::bind(13, (void**)&oResizeBuffers, resizeBuffersHook) != kiero::Status::Success) {
			Utils::DebugF("Failed to hook Resize Buffers Hook!");
			return;
		}
		Utils::DebugF("Successfully hooked Resize Buffers Hook!");
		if (kiero::bind(8, (void**)&oPresent, presentCallback) != kiero::Status::Success) {
			Utils::DebugF("Failed to hook Present D3D11 Hook!");
			return;
		}
		Utils::DebugF("Successfully hooked Present D3D11 Hook!");
	}
}

void SwapChainHook::unject() {
	kiero::shutdown();
}