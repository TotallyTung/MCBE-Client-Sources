#pragma once
#include "./Fonts.h"
#include "imgui/imfx.h"

void* oPresent;
bool initContext = false;
ID3D11Device* d3d11Device = nullptr;
ID3D12Device5* d3d12Device = nullptr;
int kairunWidth = 0;
int kairunHeight = 0;

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	d3d11Device->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	d3d11Device->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}


std::map<std::string, ImColor> colors =
{
	{"0", {0, 0, 0}},
	{"1", {0, 0, 0xAA}},
	{"2", {0, 0xAA, 0}},
	{"3", {0, 0xAA, 0xAA}},
	{"4", {0xAA, 0, 0}},
	{"5", {0xAA, 0, 0xAA}},
	{"6", {0xFF, 0xAA, 0}},
	{"7", {0xAA, 0xAA, 0xAA}},
	{"8", {0x54, 0x54, 0x54}},
	{"9", {0x54, 0x54, 0xff}},
	{"a", {0x54, 0xff, 0x54}},
	{"b", {0x54, 0xff, 0xff}},
	{"c", {0xff, 0x54, 0x54}},
	{"d", {0xff, 0x54, 0xff}},
	{"e", {0xff, 0xff, 0x54}},
	{"f", {0xff,0xff,0xff}},
	{"g", {0xef, 0xce, 0x16}},
	{"h", {0xe2, 0xd3, 0xd1}},
	{"i", {0xce, 0xc9, 0xc9}},
	{"j", {0x44, 0x39, 0x3a}},
	{"m", {0x96, 0x15, 0x06}},
	{"n", {0xb4, 0x68, 0x4d}},
	{"p", {0xde, 0xb0, 0x2c}},
	{"q", {0x11, 0x9f, 0x36}},
	{"s", {0x2c, 0xb9, 0xa8}},
	{"t", {0x20, 0x48, 0x7a}},
	{"u", {0x9a, 0x5c, 0xc5}},
	{"t", {0x20, 0x48, 0x7a}}
};

void callRender() {
	ImGui::GetIO().FontGlobalScale = 0.5f;

	//ModuleExternal->onImGuiRender();

	ImGUIRenderEvent event{};
	event.cancelled = nullptr;
	//Game::GetInstance()->moduless->DispatchEvent(&event);
	//onImGuiRender();
	DispatchEvent(&event);


	if (Game::Initializing) {
		auto windowSize = Game::RenderInfo::windowSize;
		auto font = ImGui::GetIO().Fonts->Fonts[2];
		auto font2 = ImGui::GetIO().Fonts->Fonts[1];
		auto drawList = ImGui::GetBackgroundDrawList();

		drawList->AddRectFilled(ImVec2(0, 0), ImVec2(windowSize.x, windowSize.y), ImColor(5, 8, 13));

		const float textSize = 20.f;
		auto textHeight = font->CalcTextSizeA(textSize, FLT_MAX, -1, (Game::RenderInfo::bootLogLOL + "\n").c_str()).y;
		auto textPos = ImVec2(5, windowSize.y - textHeight - 110);
		auto str = Game::RenderInfo::bootLogLOL + "";
		int posX = textPos.x;
		{
			ImColor color = colors["f"];
			bool section = false;
			for (size_t i = 0; i < str.length();)
			{
				int cplen = 1;
				if ((str[i] & 0xf8) == 0xf0) cplen = 4;
				else if ((str[i] & 0xf0) == 0xe0) cplen = 3;
				else if ((str[i] & 0xe0) == 0xc0) cplen = 2;
				if ((i + cplen) > str.length()) cplen = 1;
				std::string c = str.substr(i, cplen);
				i += cplen;
				if (section) {
					section = false;
					if (colors.find(c) != colors.end()) {
						color = colors[c];
					}
					continue;
				}
				if (c == "\n") {
					color = colors["f"];
					textPos.x = posX;
					textPos.y += font->CalcTextSizeA(textSize, FLT_MAX, -1, "").y + 2.f;
				}
				else if (c == "\uFF82\uFF67") {
					section = true;
					continue;
				}
				auto textWidth = font->CalcTextSizeA(textSize, FLT_MAX, -1, c.c_str()).x;
				drawList->AddText(font, textSize, textPos, color, c.c_str());
				textPos.x += textWidth;
			}
		}

		return;
	}
	else if (SplashScreen::showSplash) {
		SplashScreen::Render();
		return;
	}
	if (SplashScreen::showFlash) {
		SplashScreen::RenderFlash();
	}

	if (Game::GetInstance() == nullptr) return;
	if (Game::openClickGUI) {
		//ImGui::Begin("", NULL, ImGuiWindowFlags_NoCollapse);
	    
		//ImFX::AddBlur(12.f);
		//ImFX::End();
		//ImGui::SliderFloat("TimeChanger", &DayTimeValue, 0, 1, "%.2f");
		//ImGui::SliderFloat("SlowSwing", &Game::Core::SwingSpeed, 1, 70, "%.2f");
		//ImGui::End();
		ImGui::ShowDemoWindow();
	}
	ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::Begin("ConnectInfo", NULL, ImGuiWindowFlags_NoCollapse);
	auto pos = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	ImGui::Text("Host: %s\nIP: %s\nPort: %d",
		Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->host,
		Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->ipv4,
		Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->port
	);
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(40, 40), ImGuiCond_Once);
	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoCollapse);
	ImGui::SliderFloat("SlowSwing", &Game::Core::SwingSpeed, 1, 70, "%.2f");

	//ImGui::Text("Host: %s\nIP: %s\nPort: %d",
		//Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->host,
		//Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->ipv4,
		//Game::GetInstance()->loopbackPacketSender->NetworkSystem->rakNet->raknetConnector->port
	//);
	ImGui::End();
}

void loadFonts() {
	auto config = ImFontConfig();
	config.SizePixels = 40.f;
	config.OversampleH = 2;
	config.OversampleV = 2;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ProductSans_compressed_data, ProductSans_compressed_size, 48.f);
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(Mojangles_compressed_data_base85, 48.f);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 18.f);
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(IconFont_compressed_data_base85, 80.f);
	ImGui::GetIO().Fonts->AddFontDefault(&config);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(14.0f, 8.5f);
	style.WindowRounding = 8.f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(11.0f, 4.599999904632568f);
	style.FrameRounding = 6.300000190734863f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.8799999952316284f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.2800000011920929f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1019607856869698f, 0.09803921729326248f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.8999999761581421f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549295425415f, 0.47843137383461f, 0.2549019753932953f, 0.1620000004768372f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1215686276555061f, 0.1176470592617989f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.02232497371733189f, 0.4334763884544373f, 0.3410469591617584f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.01960784383118153f, 0.5254902243614197f, 0.4117647111415863f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1019607856869698f, 0.09803921729326248f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.4699999988079071f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08627451211214066f, 0.1490196138620377f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2599999904632568f, 0.5899999737739563f, 0.9800000190734863f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.239999994635582f, 0.5199999809265137f, 0.8799999952316284f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2599999904632568f, 0.5899999737739563f, 0.9800000190734863f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.1400000005960464f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.8600000143051147f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.003536617383360863f, 0.4120171666145325f, 0.3201881647109985f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.005691769998520613f, 0.442060112953186f, 0.343961775302887f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.01960784383118153f, 0.5254902243614197f, 0.4117647111415863f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.03999999910593033f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.4300000071525574f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);

	//LoadTextureFromFile("C:\\Users\\Rasky1\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\vope\\otu.png", &Game::RenderInfo::kairunTexture, &kairunWidth, &kairunHeight);

}

HRESULT D3D12_PresentDetour(IDXGISwapChain3* swapchain, UINT syncInterval, UINT flags) {
	Game::RenderInfo::window = FindWindowA(nullptr, "Minecraft");
	if (!Game::RenderInfo::window)
		Game::RenderInfo::window = FindWindowA(nullptr, "Minecraft: Developer Edition");
	if (!Game::RenderInfo::window)
		return CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
	RECT rect;
	if (GetWindowRect(Game::RenderInfo::window, &rect)) {
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		Game::RenderInfo::windowSize = Vector2<float>(width, height);
	}
	if (!SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
			swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
		}
		return CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
	}
	ID3D11DeviceContext* ppContext = nullptr;
	d3d11Device->GetImmediateContext(&ppContext);
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	ID3D11RenderTargetView* mainRenderTargetView;
	d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
	pBackBuffer->Release();
	if (!initContext) {
		ImGui::CreateContext();
		loadFonts();
		ImGui_ImplWin32_Init(Game::RenderInfo::window);
		ImGui_ImplDX11_Init(d3d11Device, ppContext);
		initContext = true;
	};
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	callRender();
	ImGui::EndFrame();

	ImGui::Render();
	ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mainRenderTargetView->Release();
	d3d11Device->Release();
	return CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
}


class DirectXHook {
public:
	static bool Initialize() {
		if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success && kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			logF("['Hidden', DirectX] Failed to create hook\n");
			return false;
		}
		const int index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
		kiero::bind(index, (void**)&oPresent, D3D12_PresentDetour);
		logF("[DirectX] Successfully Enabled hook\n");
		return true;
	}
	static bool Unbind() {
		uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
		kiero::unbind(index);
		uint16_t index2 = kiero::getRenderType() == kiero::RenderType::D3D12 ? 145 : 13;
		kiero::unbind(index2);
		return true;
	}
};