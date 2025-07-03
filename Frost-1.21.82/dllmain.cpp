// dllmain.cpp : Defines the entry point for the DLL application.

#define NOMINMAX
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <array>
#include <lmcons.h>
#include <intrin.h>
#include <sstream>
#include <xutility>
#include <string>
#include <algorithm>
#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <typeindex>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include "winrt/windows.applicationmodel.core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.Foundation.h"
#include <CoreWindow.h>
#include "winrt/Windows.UI.Core.h"
#include "winrt/windows.system.h"
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <dxgi.h>
#include <urlmon.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

static inline HWND EngineWindow = nullptr;

using namespace winrt::Windows::UI::Notifications;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel::Core;

class Module;
class ConfigurationManager;
class CommandManager;
class MainClient { public: static inline std::atomic<bool> g_isRunning{ true }; };
std::vector<std::shared_ptr<Module>> mModules;
std::vector<std::string> mCategories;
std::vector<std::string> mFrostGUICategories;

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/imgui/image/stb_image.h"

#include "Libraries/kiero/kiero.h"
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/imgui_internal.h"
#include "Libraries/imgui/backends/imgui_impl_win32.h"
#include "Libraries/imgui/backends/imgui_impl_dx11.h"
#include "Libraries/imgui/backends/imgui_impl_dx12.h"
#include "Libraries/glm/glm/glm.hpp"
#include "Libraries/glm/glm/ext/matrix_transform.hpp"
#include "Libraries/json/json.hpp"
#include "Libraries/libhat/include/libhat.hpp"
#include "Libraries/libhat/include/libhat/Access.hpp"
#include "Libraries/libhat/include/libhat/Callable.hpp"
#include "Libraries/libhat/include/libhat/Concepts.hpp"
#include "Libraries/libhat/include/libhat/CompileTime.hpp"
#include "Libraries/libhat/include/libhat/Defines.hpp"
#include "Libraries/libhat/include/libhat/FixedString.hpp"
#include "Libraries/libhat/include/libhat/MemoryProtector.hpp"
#include "Libraries/libhat/include/libhat/Process.hpp"
#include "Libraries/libhat/include/libhat/Result.hpp"
#include "Libraries/libhat/include/libhat/Scanner.hpp"
#include "Libraries/libhat/include/libhat/Signature.hpp"
#include "Libraries/libhat/include/libhat/StringLiteral.hpp"
#include "Libraries/libhat/include/libhat/Traits.hpp"
#include "Libraries/minhook/MinHook.h"
#include "Libraries/entt/entt.hpp"
#include "Client/Utils/Math/Keys/Keys.h"
#include "Client/Utils/Math/TextFormat.h"
#include "Client/Utils/Math/Color.h"
#include "Client/Utils/Math/Vector2.h"
#include "Client/Utils/Math/Vector3.h"
#include "Client/Utils/Math/Vector4.h"
#include "Client/Utils/Math/MathUtil.h"
#include "Client/Utils/Math/AABB.h"

#include "Memory/Manager/Console.hpp"
#include "Memory/Memory.h"
#include "Memory/Manager/SigManager.h"
#include "Memory/Manager/OffsetManager.h"

#include "Memory/SDK/Enums/EnumsInclude.hpp"
#include "Memory/SDK/Events/EventsInclude.h"
#include "Memory/SDK/Actor/Utilities/ActorType.h"
#include "Memory/SDK/Actor/Utilities/ActorDataIDs.h"
#include "Memory/SDK/Core/Bedrock/mce/mce.h"
#include "Memory/SDK/Actor/Utilities/ActorFlags.h"
#include "Memory/SDK/Actor/Components/PlayerBlockActionData.h"
#include "Memory/SDK/Actor/Components/PlayerBlockActions.h"
#include "Memory/SDK/Actor/Components/PlayerPositionModeComponent.h"
#include "Memory/SDK/Actor/Utilities/ActorLinkType.h"
#include "Memory/SDK/Actor/ID/ActorRuntimeID.h"
#include "Memory/SDK/Render/MinecraftUIRenderContext.h"
#include "Memory/SDK/Render/Controls/Components/UIComponent.h"
#include "Memory/SDK/Core/Bedrock/Bedrock.h"
#include "Memory/SDK/Core/OwnerPtr.h"
#include "Memory/SDK/Core/WeakRef.h"
#include "Memory/SDK/Actor/Level/Tick.h"
#include "Memory/SDK/Render/HashedString.h"
#include "Memory/SDK/Core/Bedrock/json/Json.h"
#include "Memory/SDK/Packet/Network/PacketSender.h"
#include "Memory/SDK/Actor/Level/Components/BlockComponentStorage.hpp"
#include "Memory/SDK/Block/Block.h"
#include "Memory/SDK/Block/Actor/BlockActor.h"
#include "Memory/SDK/Container/ArmorSlot/ArmorSlot.h"
#include "Memory/SDK/Container/Item/Item.h"
#include "Memory/SDK/Container/Item/ItemStack.h"
#include "Memory/SDK/Container/Inventory/Inventory.h"
#include "Memory/SDK/Container/SimpleContainer.h"
#include "Memory/SDK/Container/Inventory/PlayerInventory.h"
#include "Memory/SDK/Container/Manager/ContainerManagerModel.h"
#include "Memory/SDK/Actor/Skin/SerializedSkin.h"
#include "Memory/SDK/Actor/effect/MobEffectInstance.h"
#include "Memory/SDK/Container/ContainerScreenController.h"
#include "Memory/SDK/Actor/Ability.h"
#include "Memory/SDK/Actor/Actor.h"
#include "Memory/SDK/Actor/ItemActor.h"
#include "Memory/SDK/Actor/Level/Level.h"
#include "Memory/SDK/Packet/Packet.h"
#include "Memory/SDK/Block/Source/BlockSource.h"
#include "Memory/SDK/GameMode/GameMode.h"
#include "Memory/SDK/Actor/Mob.h"
#include "Memory/SDK/Actor/Player.h"
#include "Memory/SDK/Core/ClientInstance.h"
#include "Memory/SDK/Render/Screen/ScreenView.h"
#include "Memory/SDK/Render/Context/Tessellator/Tessellator.h"

#include "Client/Event/Event.hpp"
#include "Memory/Manager/InstanceManager.h"
#include "Client/Utils/Render/Frame/FrameUtil.h"
#include "Client/Utils/Util.h"
#include "Client/Utils/Actor/ActorUtils.h"
#include "Client/Utils/Actor/Chat/ChatUtil.h"
#include "Client/Utils/Actor/Motion/MotionUtil.h"
#include "Client/Utils/Render/ImRenderUtil.hpp"
#include "Client/Utils/Render/EasingUtil.hpp"
#include "Client/Utils/Misc/BlockUtil.h"
#include "Client/Utils/Misc/ItemUtil.h"
#include "Client/Utils/Misc/PacketUtil.h"
#include "Client/Utils/Time/TimeUtil.h"
#include "Client/Utils/File/FileUtil.h"
#include "Client/Utils/Render/Color/ColorUtil.h"
#include "Memory/Graphics/Graphics.h"
#include "Memory/Graphics/DirectX/DirectX.hpp"
#include "Client/Utils/Render/RenderUtil.hpp"
#include "Client/Utils/Render/RenderUtil3D.hpp"

#include "Client/Modules/Module.h"

std::shared_ptr<Module> getModuleByName(const std::string& name) 
{
    std::string nameLower(name);
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    auto it = std::find_if(mModules.begin(), mModules.end(), [&](const std::shared_ptr<Module>& m) {
        std::string moduleNameLower = m->getName();
        std::transform(moduleNameLower.begin(), moduleNameLower.end(), moduleNameLower.begin(), ::tolower);
        moduleNameLower.erase(std::remove(moduleNameLower.begin(), moduleNameLower.end(), ' '), moduleNameLower.end());
        return nameLower == moduleNameLower;
        });

    return it != mModules.end() ? *it : nullptr;
}

template<typename T>
T* getModuleByType() 
{
    for (const auto& module : mModules) 
    {
        if (dynamic_cast<T*>(module.get())) 
        {
            return static_cast<T*>(module.get());
        }
    }
    return nullptr;
}

#include "Client/Configuration/ConfigManager.h"

ConfigurationManager mConfigMgr;

#include "Client/Command/Command.h"
#include "Client/Command/CommandManager.h"

CommandManager mCommandMgr;

#include "Memory/Hooks/HookManager.h"
#include "Client/Modules/ModuleManager.h"

#include "Client/Client.h"

void InitializeClient() 
{
    Client::Initialize(); // Load Hooks Modules Etc
}

DWORD APIENTRY shutdownThread(HMODULE lpParam)
{
    while (Client::g_isRunning.load())
    {
        Util::setTitle(Util::combine("Frost [impl_win32] (Debug) | ", static_cast<int>(GlobalVariables::mFPS), " FPS | ", static_cast<int>(Graphics::Memory::getPercentUsed()), "% Memory | ", Graphics::GPU::getUsedGraphicsCardName()));

        if (GlobalVariables::mKey[Keys::END])
        {
            Client::g_isRunning.store(false);
        }

        // Add a small delay to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Client::Shutdown();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    FreeLibraryAndExitThread(lpParam, 1); // Eject the client completly.
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitializeClient, hModule, 0, nullptr);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)shutdownThread, hModule, 0, 0);
    }

    /*if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        
    }*/

    return TRUE;
}

