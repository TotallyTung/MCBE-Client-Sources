#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <string>
#include <Psapi.h>
#include <ostream> // New
#include <fstream> // New
#include <sstream>
#include <thread>
#include <queue>
#include <map>
#include <iostream>
#include <mutex>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Foundation.h>
#include "ws2def.h"
#include "shellapi.h"
using namespace winrt::Windows::UI::Notifications;
class Module;
std::vector<Module*> modules = std::vector<Module*>();
std::vector<std::string> categories;

// DirectX

#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Libs/minhook/minhook.h"
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "imgui/impl/imgui_impl_dx11.h"
#include "imgui/impl/imgui_impl_dx12.h"

#include "imgui/imgui_internal.h"

#include "Libs/xorstr.hpp"

// Include meth
#include "SDK/Math/UIColor.h"
#include "SDK/Math/MCTextFormat.h"
#include "SDK/Math/TextInput.h"
#include "SDK/Math/Keys.h"
#include "SDK/Math/Math.h"
#include "SDK/Math/Vector2.h"
#include "SDK/Math/Vector3.h"
#include "SDK/Math/Vector4.h"
#include "SDK/Math/AABB.h"

#include "Utils/Utils.h"
#include "Utils/TimeUtil.h"
#include "Utils/ChatUtil.h"
#include "Utils/ColorUtil.h"
#include "Utils/MemoryUtils.h"
#include "Utils/DrawUtils.h"
#include "Utils/Console.h"
#include "Utils/ImRenderUtil.h"
#include "Utils/EasingUtil.h"
#include "SDK/TextHolder.h"
#include "SDK/Struct/MinecraftUIRenderContext.h"
#include "SDK/Struct/Packet.h"
#include "SDK/UIControl.h"
#include "SDK/Components/StateVectorComponent.h"
#include "SDK/Struct/GameMode.h"
#include "SDK/Struct/Player.h"
#include "SDK/Struct/ScreenView.h"
#include "SDK/Struct/Mob.h"
#include "SDK/Struct/LevelRenderer.h"
#include "SDK/Packets/UpdateBlockPacket.h"
#include "SDK/Packets/TextPacket.h"
#include "SDK/Core/ClientInstance.h"
#include "Client/Game.h"
#include "Client/GUI/SplashScreen.h"
#include "SDK/Event/Events.h"
#include "Client/Module/Module.h"
Module* getModuleByName(const std::string& name) {
    std::string nameLower(name);
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    auto it = std::find_if(modules.begin(), modules.end(), [&](Module* m) {
        std::string moduleNameLower = m->getName();
        std::transform(moduleNameLower.begin(), moduleNameLower.end(), moduleNameLower.begin(), ::tolower);
        moduleNameLower.erase(std::remove(moduleNameLower.begin(), moduleNameLower.end(), ' '), moduleNameLower.end());
        return nameLower == moduleNameLower;
        }
    );
    return it != modules.end() ? *it : nullptr;
}


#include "Client/FuncHook.h"
#include "Client/GUI/DirectXHook.h"

#include "Client/Module/ModuleHandler.h"