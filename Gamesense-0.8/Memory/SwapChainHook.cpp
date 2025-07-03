#include "SwapChainHook.h"

#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_3.h>

#include "../include/Kiero/Kiero.h"
#include "../include/ImGui/imgui.h"
#include "../include/ImGui/imgui_internal.h"
#include "../include/ImGui/imgui_impl_dx11.h"
#include "../include/ImGui/imgui_impl_dx12.h"
#include "../include/ImGui/imgui_impl_win32.h"

#include "../Utils/Utils.h"
#include "../Utils/ImGuiUtils.h"
#include "../Utils/Logger.h"

#include "GameData.h"

// FONTS
#include "../include/Fonts/ProductSansRegular.h"
#include "../Gamesense/Module/ModuleManager.h"

bool imguiInit = false;
bool showImguiDemo = false;

typedef HRESULT(__thiscall* PresentD3D12)(IDXGISwapChain3*, UINT, UINT);
PresentD3D12 oPresentD3D12;

void render(ImDrawList* d) {
	// render test
	moduleMgr->onImGuiRender(d);


	ImVec2 windowSize = ImVec2(g_Data.getClientInstance()->getGuiData()->windowSizeReal.x, g_Data.getClientInstance()->getGuiData()->windowSizeReal.y);

	auto clickGUIMod = moduleMgr->getModule<ClickGUIMod>();
	if (clickGUIMod->isEnabled())
	{
		if (clickGUIMod->hasOpenedGUI)
		{
			ClickGui::imguirender(d);
		}
	}


	auto notificationsMod = moduleMgr->getModule<Notifications>();
	int notifSize = notificationsMod->notifSize;

	// Notifications Render
	float fps = ImGui::GetIO().Framerate;

	float notificationTextSize = ImGuiUtils::getTextHeight(d, "A", notifSize);
	float yStart = ImGui::GetIO().DisplaySize.y - notificationTextSize * 3.f;
	auto watermark = moduleMgr->getModule<Watermark>();
	auto notifications = g_Data.getAllNotification();
	if (notificationsMod->isEnabled() && !watermark->isFirstTime) {
		//float yPos = windowSize.y - notificationTextSize * 3.f;

		for (int i = 0; i < notifications.size(); i++) {

			if (notifications[i]->duration == notifications[i]->maxDuration) {
				notifications[i]->PosX = windowSize.x + 2.5f;
				notifications[i]->PosY = yStart;
				notifications[i]->VelocicyX = 0.f;
			}
			char sec[10];
			snprintf(sec, 10, " (%.2fs)", notifications[i]->duration);
			//std::string text = notifications[i]->title + sec;
			std::string text = notifications[i]->title;

			float textWidth = ImGuiUtils::getTextWidth(d, text.c_str(), notifSize);
			float textHeight = ImGuiUtils::getTextHeight(d, text.c_str(), notifSize);

			float KC1 = windowSize.x + 2.5f - (windowSize.x - textWidth - 5.f);
			float KC2 = windowSize.x + 2.5f - notifications[i]->PosX;
			float idk = (KC2 / KC1);
			if (idk > 1.f) idk = 1.f;

			float fraction = notifications[i]->duration / notifications[i]->maxDuration;
			//d->AddShadowRect(ImVec2(notifications[i]->PosX, notifications[i]->PosY), ImVec2(notifications[i]->PosX + textWidth, notifications[i]->PosY + textHeight), ImColor(0, 0, 0, (int)(175 * idk)), 40.f, ImVec2(0.f, 0.f), ImDrawFlags_ShadowCutOutShapeBackground);
			if (notificationsMod->mode.getSelectedValue() == 1) {
				ImColor color = ImColor(ColorUtil::interfaceColor(1).r, ColorUtil::interfaceColor(1).g, ColorUtil::interfaceColor(1).b, idk);
				ImGuiUtils::renderRectFilled(d, ImVec4(notifications[i]->PosX - 5.f, notifications[i]->PosY - 5.f, notifications[i]->PosX + textWidth + 10.f, notifications[i]->PosY + textHeight + 5.f), ImColor(0, 0, 0, int(100 * idk)));
				ImGuiUtils::renderRectFilled(d, ImVec4(notifications[i]->PosX - 5.f, notifications[i]->PosY + textHeight + 5.f, notifications[i]->PosX + (textWidth + 10.f) * (1.f-fraction), notifications[i]->PosY + textHeight + 7.f), color);
			}
			if (notificationsMod->mode.getSelectedValue() == 2) {
				ImColor color = ImColor(ColorUtil::interfaceColor(1).r, ColorUtil::interfaceColor(1).g, ColorUtil::interfaceColor(1).b, idk * 0.65f);
				ImGuiUtils::renderRect(d, ImVec4(notifications[i]->PosX - 5.f, notifications[i]->PosY - 5.f, notifications[i]->PosX + textWidth + 10.f, notifications[i]->PosY + textHeight + 5.f), ImColor(0, 0, 0, int(255 * idk)), 1.f, 10.f);
				ImGuiUtils::renderRectFilled(d, ImVec4(notifications[i]->PosX - 5.f, notifications[i]->PosY - 5.f, notifications[i]->PosX + textWidth + 10.f, notifications[i]->PosY + textHeight + 5.f), ImColor(0, 0, 0, int(100 * idk)), 10.f);
				ImGuiUtils::renderRectFilled(d, ImVec4(notifications[i]->PosX - 5.f, notifications[i]->PosY - 5.f, notifications[i]->PosX + (textWidth + 10.f) * (1.f - fraction), notifications[i]->PosY + textHeight + 5.f), color, 10.f);
			}
			//d->AddText(NULL, notificationTextSize, ImVec2(notifications[i]->PosX, notifications[i]->PosY), ImColor(255, 255, 255, (int)(255 * idk)), text.c_str());
			if (notificationsMod->mode.getSelectedValue() == 0) ImGuiUtils::renderGradientText(d, ImVec2(notifications[i]->PosX, notifications[i]->PosY), text.c_str(), notifSize, 0, idk);
			else ImGuiUtils::renderText(d, ImVec2(notifications[i]->PosX, notifications[i]->PosY), text.c_str(), ImColor(255, 255, 255, int(255.f*idk)), notifSize);
			if (notifications[i]->duration > 0.f) {
				if (fps < 1.f) fps = 1.f;
				notifications[i]->duration -= 1.f / fps;
				if (notifications[i]->duration < 0.f) notifications[i]->duration = 0.f;

				notifications[i]->PosX += ((windowSize.x - textWidth - 35.f) - notifications[i]->PosX) * 0.1f;
				notifications[i]->PosY += (yStart - notifications[i]->PosY) * 0.1f;

			}
			else {
				if (notifications[i]->PosX < windowSize.x + 2.5f) {
					notifications[i]->PosX += ((windowSize.x + 20.f) - notifications[i]->PosX) * 0.1f;
					//notifications[i]->PosX += notifications[i]->VelocicyX;
					//notifications[i]->VelocicyX += 0.5f;
					notifications[i]->PosY += (yStart - notifications[i]->PosY) * 0.1f;
				}
				/*
				else {
					notifications.erase(notifications.begin() + i);
					i--;
				}
				*/
				if (notifications[i]->PosX >= windowSize.x + 2.5f) {
					notifications.erase(notifications.begin() + i);
					i--;
				}
			}

			yStart -= (notificationTextSize * 2.f) * idk;
		}
	}
}

HRESULT presentCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags) {

	//if (!client->isRunning) return oPresentD3D12(pSwapChain, syncInterval, flags);
	//if (!moduleMgr->isInitialized()) return oPresentD3D12(pSwapChain, syncInterval, flags);
	if (g_Data.getClientInstance() == nullptr) return oPresentD3D12(pSwapChain, syncInterval, flags);
	if (g_Data.getClientInstance()->getGuiData() == nullptr) return oPresentD3D12(pSwapChain, syncInterval, flags);

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

		if (!imguiInit) {
			if (ImGui::GetCurrentContext() == nullptr) {
				ImGui::CreateContext();
			}

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//ImGui::StyleColorsDark();
			ImGui::StyleColorsDark();
			io.IniFilename = NULL;

			//io.Fonts->AddFontFromMemoryTTF((void*)RobotoRegular, sizeof(RobotoRegular), 40.f);
			//io.Fonts->AddFontFromMemoryTTF((void*)ComfortaaRegular, sizeof(ComfortaaRegular), 40.f);
			io.Fonts->AddFontFromMemoryTTF((void*)ProductSansRegular, sizeof(ProductSansRegular), 40.f);

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			imguiInit = true;

		}
		pBackBuffer->Release();
		//ImGuiContext* g = ImGui::GetCurrentContext();
		//g->HoveredIdAllowOverlap = true;
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(ImVec2(g_Data.getClientInstance()->getGuiData()->windowSizeReal.x, g_Data.getClientInstance()->getGuiData()->windowSizeReal.y));

		//ImGui::GetIO().DisplaySize.x = Game.getClientInstance()->getGuiData()->windowSizeReal.x;
		//ImGui::GetIO().DisplaySize.y = Game.getClientInstance()->getGuiData()->windowSizeReal.y;

		ImGui::NewFrame();
		// Render in here;
		{
			ImDrawList* d = ImGui::GetBackgroundDrawList();

			render(d);
		}

		ImGui::EndFrame();
		ImGui::Render();

		//Finish ImGui draw
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(drawData);

		ppContext->Flush();
		dxgiBackBuffer->Release();
		ppContext->Release();
		if (mainRenderTargetView) mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device))))
	{
		static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
	}
	return oPresentD3D12(pSwapChain, syncInterval, flags);
}

void SwapChainHook::init() {
	bool isDx11 = false;
	if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success) {
		if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			logF("Failed to initialize kiero!");
			return;
		}
		isDx11 = true;
	}
	if (!isDx11) {
		if (kiero::bind(140, (void**)&oPresentD3D12, presentCallback) != kiero::Status::Success) {
			logF("Failed to hook Present D3D12!");
			return;
		}
	}
	else {
		if (kiero::bind(8, (void**)&oPresentD3D12, presentCallback) != kiero::Status::Success) {
			logF("Failed to hook Present D3D11!");
			return;
		}
	}
	if (!isDx11) {
		logF("Hooked Present D3D12.");
	}
	else {
		logF("Hooked Present D3D11.");
	}
}

void SwapChainHook::eject() {
	kiero::unbind(140);
	kiero::shutdown();
}