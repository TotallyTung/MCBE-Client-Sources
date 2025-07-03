#include "Manager.h"
#include "Client.h"
#include "../Include/imgui/imgui.h"
#include <vector>
#include "../Include/imgui/font_awesome_5.h"
#include <Windows.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>


/*  NOTIFICATION SHIT HERE  */
auto Manager::ShowToastNotification(const std::string& title, const std::string& text) -> void {
	using namespace winrt::Windows::UI::Notifications;
	using namespace winrt::Windows::Data::Xml::Dom;

	ToastNotificationManager::CreateToastNotifier();
	XmlDocument toastXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText03);
	XmlNodeList textElements = toastXml.GetElementsByTagName(L"text");
	textElements.Item(0).InnerText(winrt::to_hstring(title));
	textElements.Item(1).InnerText(winrt::to_hstring(text));

	ToastNotification toast(toastXml);
	ToastNotificationManager::CreateToastNotifier().Show(toast);
}

void Manager::ShowNotificationWithDelay(const std::string& title, const std::string& text, int delayInSeconds) {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::seconds(delayInSeconds));
	ShowToastNotification(title, text);
		}).detach();
}

Manager::Manager(Client* c) {

	this->client = c;
	this->isRunning = true;

	auto hooksData = this->initHooks();
	
	switch (hooksData.first) {

		case MethodStatus::Success:
			
			this->initCategories();

		break;

		case MethodStatus::Error:
			
			Utils::debugOutput(hooksData.second);
			this->isRunning = false;

		break;

		case MethodStatus::Failed:
			
			this->isRunning = false;
			this->uninjectDLL();

		break;
	}

	while (this->isRunning) {

		for (auto category : this->categories) {

			category->baseTick();

		};

	};

	this->uninjectDLL();

};

//Hook Includes
#include "../Hooks/LoopbackPacketSender/HookLoopbackPacketSender.h"
#include "../Hooks/ClientInstance/HookClientInstance.h"
#include "../Hooks/DirectX/DirectXHook.h"
#include "../Hooks/SwingHook/MobGetCurrentSwingHook.h"
#include "../Hooks/RakPeerHook/RakPeerHook.h"
#include "../Hooks/MouseButton/HookMouse.h"
#include "../Hooks/Key/HookKey.h"
#include "../Hooks/ActorRenderDispatcher/ActorRenderDispatcher.h"
#include "../Hooks/ActorTick/ActorTick.h"

auto Manager::initHooks(void) -> StatusData {

	if (MH_Initialize() != MH_OK)
		return StatusData(MethodStatus::Error, "Failed to Initialize MinHook");

	new Hook_ClientInstance(this);
	new Hook_DirectX(this);
	new Hook_MobGetCurrentSwing(this);
	new Hook_RakPeer(this);
	new Hook_Mouse(this);
	new Hook_Key(this);
	new Hook_LoopbackPacketSender(this);
	new Hook_ActorRender(this);
	new Hook_Actor(this);

	for (auto hook : this->hooks) {

		auto data = hook->init();
		Utils::debugOutput(data.second);

	};

	return StatusData(MethodStatus::Success, "Finished initializing hooks!");

};

//Include Modules


/*Combat*/
#include "Modules/Combat/Killaura.h"


/*Movement*/
#include "Modules/Movement/AirJump.h"
#include "Modules/Movement/AutoSprint.h"
#include "Modules/Movement/AutoJump.h"
#include "Modules/Movement/Jetpack.h"

/*Player*/
#include "Modules/Player/Jesus.h"

/*Visuals*/
#include "Modules/Visual/ArrayList.h"
#include "Modules/Visual/TabGui.h"
#include "Modules/Visual/Watermark.h"
#include "Modules/Visual/CreditsModule.h"
#include "Modules/Visual/ClickGui.h"
#include "Modules/Visual/Notifications.h"

/*World*/
#include "Modules/World/Scaffold.h"

/*Misc*/
#include "Modules/Other/TestModule.h"
#include "Modules/Other/NetSkip.h"
#include "Modules/Other/Uninject.h"
#include "Modules/Other/ClickGui2.h"


auto Manager::initCategories(void) -> void {

	
	AllocConsole();
	SetConsoleTitleA("console output");

	FILE* file;
	freopen_s(&file, "CONIN$", "r", stdin);
	freopen_s(&file, "CONOUT$", "w", stdout);

	std::cout << "Working" << std::endl;
	 //remove this if we want for console log

	Utils::downloadImages();

	/*/
	Vec2<float> imagePos(100.0f, 100.0f); // Position where you want to draw the image
	Vec2<float> ImageDimension(100.0f, 100.0f); // Original dimensions of the image (1920x1080)
	Vec2<float> uvPos(100.0f, 100.0f); // UV coordinate position (assuming starting from top-left)
	Vec2<float> uvSize(100.0f, 1000.0f); // UV coordinate size (assuming covering the whole image)

	std::string imagePath = Utils::getResourcePath() + "\\mart.png";
	RenderUtils::drawImage(imagePath, imagePos, ImageDimension, uvPos, uvSize);
	/*/
	ShowToastNotification("Air Client", "Successfully Injected Air!");
	ShowToastNotification("Authorization", "Welcome back tozic!");


	/* Combat */
	auto combat = new Category(this, "Combat");
	combat->index = 0;
	new Killaura(combat);

	/* Movement */
	auto move = new Category(this, "Move");
	move->index = 1;
	new AirJump(move);
	new AutoJump(move);
	new AutoSprint(move);
	new Jetpack(move);

	/* Player */
	auto player = new Category(this, "Player");
	player->index = 2;
	new Jesus(player);

	/* Visuals */
	auto visuals = new Category(this, "Visuals");
	visuals->index = 3;
	new ArrayList(visuals);
	new ClickGui(visuals);
	new Watermark(visuals);
	new CreditsModule(visuals);
	new NotificationsModule(visuals);
	new TabGui(visuals);

	/* World */
	auto world = new Category(this, "World");
	world->index = 4;
	new Scaffold(world);

	/* Other */
	auto other = new Category(this, "Misc");
	other->index = 5;
	new TestModule(other);
	new NetSkip(other);
	new UninjectModule(other);
    new ClickGui2(other);
};

	auto Manager::uninjectDLL(void) -> void {

		DisableThreadLibraryCalls(Utils::getDll());
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);

		ShowNotificationWithDelay("Air Client", "Zen Client has been ejected!" ,0.1);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		FreeLibraryAndExitThread(Utils::getDll(), 0);
		Sleep(1000);
		MH_Uninitialize();
};
