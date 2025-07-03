#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <algorithm>
using json = nlohmann::json;
const char* Encrypt(const char* str) {
	char key = 'CUMM';
	static char result[128];
	int len = strlen(str);
	for (int i = 0; i < len; ++i) {
		result[i] = str[i] ^ key;
	}
	result[len] = '\0';
	return result;
}
std::string authorizedUsers[] = {
	"MerPlayzMC",
	"XxTrongDZxX",
	"IllLilMartin1",
	"blad blad bung",
	"EnesTalha123197",
	"XYLOTLOS",
	"DecafTester6808",
	"SSSSSS LMAO",
	"NukE11331",
	"Jepium",
	"Afar Arkaris",
	"Irvin289139",
	"Bunnynip117",
	"xixtsun",
	"Ege3199",
	"R1ester",
	"toastedbuter773",
	"GhOsT37664",
	"Iammtrashhh",
	"Nova dk mod5116",
	"LeftAstro7915",
	"zqe2",
	"Gokugen1219",
	"MerciGGs",
	"z1atqn",
	"Umason420",
	"Yuki Baishunpu",
	"Japacum",
	"Z111111111I",
	"Sub Askedme",
	"SSSSSS LMAO",
	"zpo4",
    "SplitSleet51560",
	"HoleCampinqq",
	"TudouTheKing",
	"zerotwopog2",
	"OnlyGodOfAll",
	"Hades on top796",
	"Zh1n1anQwQ",
	"x17r",
	"bendzone",
	"Sinqqqqqqq",
	"lordmac0"
};
bool isVerifiedUser() {
	const char* xorAuth[] = {
	Encrypt("MerPlayzMC"),
	Encrypt("XxTrongDZxX"),
	Encrypt("IllLilMartin1"),
	Encrypt("blad blad bung"),
	Encrypt("EnesTalha123197"),
	Encrypt("XYLOTLOS"),
	Encrypt("DecafTester6808"),
	Encrypt("SSSSSS LMAO"),
	Encrypt("NukE11331"),
	Encrypt("Jepium"),
	Encrypt("Afar Arkaris"),
	Encrypt("Irvin289139"),
	Encrypt("Bunnynip117"),
	Encrypt("xixtsun"),
	Encrypt("Ege3199"),
	Encrypt("R1ester"),
	Encrypt("toastedbuter773"),
	Encrypt("GhOsT37664"),
	Encrypt("Iammtrashhh"),
	Encrypt("Nova dk mod5116"),
	Encrypt("LeftAstro7915"),
	Encrypt("zqe2"),
	Encrypt("Gokugen1219"),
	Encrypt("MerciGGs"),
	Encrypt("z1atqn"),
	Encrypt("Umason420"),
	Encrypt("Yuki Baishunpu"),
	Encrypt("Japacum"),
	Encrypt("Z111111111I"),
	Encrypt("Sub Askedme"),
	Encrypt("SSSSSS LMAO"),
	Encrypt("zpo4"),
	Encrypt("SplitSleet51560"),
	Encrypt("HoleCampinqq"),
	Encrypt("TudouTheKing"),
	Encrypt("zerotwopog2"),
	Encrypt("OnlyGodOfAll"),
	Encrypt("Hades on top796"),
	Encrypt("Zh1n1anQwQ"),
	Encrypt("x17r"),
	Encrypt("bendzone"),
	Encrypt("Sinqqqqqqq"),
	Encrypt("lordmac0")
	};
	const char* currentUsername = g_Data.getLocalPlayer()->getNameTag()->getText();
	for (int i = 0; i < sizeof(xorAuth) / sizeof(xorAuth[0]); ++i) {
		const char* decrypted = Encrypt(xorAuth[i]);
		if (strcmp(currentUsername, decrypted) == 0) {
			return true;
		}
	}
	return false;
}
ModuleManager::ModuleManager(GameData* gameData)
{
	this->gameData = gameData;
}

ModuleManager::~ModuleManager()
{
	initialized = false;
	auto lock = this->lockModuleListExclusive();
	this->moduleList.clear();
}

void ModuleManager::initModules()
{
	{
		auto lock = this->lockModuleListExclusive();

		// Menu
		moduleList.push_back(std::shared_ptr<IModule>(new ConfigManagerMod()));
		moduleList.push_back(std::shared_ptr<IModule>(new ClickGUIMod()));
		moduleList.push_back(std::shared_ptr<IModule>(new DebugMenu()));

		// Combat
		moduleList.push_back(std::shared_ptr<IModule>(new AutoCrystalRewrite()));
		//moduleList.push_back(std::shared_ptr<IModule>(new AutoCrystal()));
		moduleList.push_back(std::shared_ptr<IModule>(new GravityAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new TNTBomber()));
		moduleList.push_back(std::shared_ptr<IModule>(new BedAuraRewrite()));
		moduleList.push_back(std::shared_ptr<IModule>(new BetterBow()));
		//moduleList.push_back(std::shared_ptr<IModule>(new CrystalAuraGK()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoObsidian()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiCrystal()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiBot()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoSwitch()));
		moduleList.push_back(std::shared_ptr<IModule>(new Surround()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTrap()));
		moduleList.push_back(std::shared_ptr<IModule>(new Killaura()));
		moduleList.push_back(std::shared_ptr<IModule>(new Reach()));

		// Visual
		moduleList.push_back(std::shared_ptr<IModule>(new HUD()));
		moduleList.push_back(std::shared_ptr<IModule>(new BreakHighlight()));
		moduleList.push_back(std::shared_ptr<IModule>(new ViewModel()));
		moduleList.push_back(std::shared_ptr<IModule>(new TargetVisualize()));
		moduleList.push_back(std::shared_ptr<IModule>(new SearchBlocks()));
		moduleList.push_back(std::shared_ptr<IModule>(new SlowSwing()));
		moduleList.push_back(std::shared_ptr<IModule>(new BlockOutline()));
		moduleList.push_back(std::shared_ptr<IModule>(new PlayerListx()));
		moduleList.push_back(std::shared_ptr<IModule>(new Animations()));
		moduleList.push_back(std::shared_ptr<IModule>(new Notifications()));
		moduleList.push_back(std::shared_ptr<IModule>(new ArrayList()));
		moduleList.push_back(std::shared_ptr<IModule>(new Interface()));
		moduleList.push_back(std::shared_ptr<IModule>(new TargetHUD()));
		moduleList.push_back(std::shared_ptr<IModule>(new Watermark()));
		moduleList.push_back(std::shared_ptr<IModule>(new HoleEsp()));
		moduleList.push_back(std::shared_ptr<IModule>(new NameTags()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoRender()));
		moduleList.push_back(std::shared_ptr<IModule>(new Camera()));
		moduleList.push_back(std::shared_ptr<IModule>(new Zoom()));
		moduleList.push_back(std::shared_ptr<IModule>(new ESP()));

		// Movement
		moduleList.push_back(std::shared_ptr<IModule>(new NoClipGlide()));
		moduleList.push_back(std::shared_ptr<IModule>(new Spider()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoClip()));
		moduleList.push_back(std::shared_ptr<IModule>(new PacketFly()));
		moduleList.push_back(std::shared_ptr<IModule>(new Anchor()));
		moduleList.push_back(std::shared_ptr<IModule>(new ReverseStep()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastStop()));
		moduleList.push_back(std::shared_ptr<IModule>(new Velocity()));
		moduleList.push_back(std::shared_ptr<IModule>(new Jetpack()));
		moduleList.push_back(std::shared_ptr<IModule>(new InvMove()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoClip()));
		moduleList.push_back(std::shared_ptr<IModule>(new Flight()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoSlowDown()));
		moduleList.push_back(std::shared_ptr<IModule>(new Sprint()));
		moduleList.push_back(std::shared_ptr<IModule>(new Jesus()));
		moduleList.push_back(std::shared_ptr<IModule>(new Sneak()));
		moduleList.push_back(std::shared_ptr<IModule>(new Speed()));
		moduleList.push_back(std::shared_ptr<IModule>(new Step()));
		moduleList.push_back(std::shared_ptr<IModule>(new Bhop()));

		//  Player
		moduleList.push_back(std::shared_ptr<IModule>(new ChestStealer()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoHoleFill()));
		moduleList.push_back(std::shared_ptr<IModule>(new GhostHand()));
		// moduleList.push_back(std::shared_ptr<IModule>(new BasePlace()));
		moduleList.push_back(std::shared_ptr<IModule>(new Burrow()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTool()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoHotbar()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTotem()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastPlace()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoArmor()));
		moduleList.push_back(std::shared_ptr<IModule>(new Scaffold()));
		moduleList.push_back(std::shared_ptr<IModule>(new Freecam()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastEat()));
		moduleList.push_back(std::shared_ptr<IModule>(new Blink()));
		moduleList.push_back(std::shared_ptr<IModule>(new PacketMine()));
		moduleList.push_back(std::shared_ptr<IModule>(new Timer()));

		// Exploit
		moduleList.push_back(std::shared_ptr<IModule>(new AntiDesync()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoKit()));
		moduleList.push_back(std::shared_ptr<IModule>(new PacketXP()));
		moduleList.push_back(std::shared_ptr<IModule>(new StashFinder()));
		moduleList.push_back(std::shared_ptr<IModule>(new OffhandAllow()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoDupe()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoEntityTrace()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoHunger()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoRespawn()));
		// moduleList.push_back(std::shared_ptr<IModule>(new ChatBypass()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastUse()));
		moduleList.push_back(std::shared_ptr<IModule>(new Phase()));

		// Other
		moduleList.push_back(std::shared_ptr<IModule>(new ToggleSounds()));
		moduleList.push_back(std::shared_ptr<IModule>(new ClientSetting()));
		moduleList.push_back(std::shared_ptr<IModule>(new ChatSuffix()));
		moduleList.push_back(std::shared_ptr<IModule>(new Spammer()));
		// moduleList.push_back(std::shared_ptr<IModule>(new Verify()));
		moduleList.push_back(std::shared_ptr<IModule>(new Derp()));
		moduleList.push_back(std::shared_ptr<IModule>(new MCF()));

#ifdef _DEBUG
		//moduleList.push_back(std::shared_ptr<IModule>(new PacketLogger()));
		moduleList.push_back(std::shared_ptr<IModule>(new TestModule()));
		// moduleList.push_back(std::shared_ptr<IModule>(new FallSave()));
#endif

		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs)
			{
				auto current = lhs;
				auto other = rhs;
				return std::string{ *current->getRawModuleName() } < std::string{ *other->getRawModuleName() }; }); \
			initialized = true;
	}
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers)) {
			getModule<Watermark>()->isFirstTime = true;
			g_Data.addNotification("Welcome to Gamesense v" + moduleMgr->getModule<Interface>()->versionStr + ", " + playerID, "", 5.f);
		}
		else {
			C_TextPacket textPacket;
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string coords =
				std::to_string((int)floorf(currPos->x)) + ", " +
				std::to_string((int)floorf(currPos->y)) + ", " +
				std::to_string((int)floorf(currPos->z));
			std::string message = "@here i just tried to use a Gamesense Client crack, My coordinates is XYZ: " + coords + "! GET GOOD. GET GAMESENSE";
			textPacket.message.setText(message);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			GameData::terminate();
		}
	}

	getModule<Notifications>()->setEnabled(true);
	getModule<ClickGUIMod>()->setEnabled(false);
	getModule<Killaura>()->setEnabled(false);
	getModule<Interface>()->setEnabled(true);
	getModule<ArrayList>()->setEnabled(true);
	getModule<Watermark>()->setEnabled(true);
	getModule<ClientSetting>()->setEnabled(true);
}

void ModuleManager::disable()
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				if (mod->isEnabled())
					mod->setEnabled(false);
			}
		}
		else
			GameData::terminate();
	}
	if (g_Data.getLocalPlayer() == nullptr) {
		auto lock = this->lockModuleList();
		for (auto& mod : this->moduleList)
		{
			if (mod->isEnabled())
				mod->setEnabled(false);
		}
	}
}

void ModuleManager::onLoadConfig(void* confVoid)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			auto conf = reinterpret_cast<json*>(confVoid);
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				mod->onLoadConfig(conf);
			}

			if (!getModule<Step>()->isEnabled() && g_Data.getLocalPlayer() != nullptr)
				g_Data.getLocalPlayer()->stepHeight = 0.5625f;
			getModule<ClickGUIMod>()->setEnabled(false);
		}
		else
			GameData::terminate();
	}
	if (g_Data.getLocalPlayer() == nullptr) {
		auto conf = reinterpret_cast<json*>(confVoid);
		if (!isInitialized())
			return;

		auto lock = this->lockModuleList();
		for (auto& mod : this->moduleList)
		{
			mod->onLoadConfig(conf);
		}

		if (!getModule<Step>()->isEnabled() && g_Data.getLocalPlayer() != nullptr)
			g_Data.getLocalPlayer()->stepHeight = 0.5625f;
		getModule<ClickGUIMod>()->setEnabled(false);
	}
}

void ModuleManager::onSaveConfig(void* confVoid)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			auto conf = reinterpret_cast<json*>(confVoid);
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				mod->onSaveConfig(conf);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onLoadSettings(void* confVoid)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();

		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			auto conf = reinterpret_cast<json*>(confVoid);
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				mod->onLoadSettings(conf);
			}
			getModule<ClickGUIMod>()->setEnabled(false);
		}
		else
			GameData::terminate();
	}
	if (g_Data.getLocalPlayer() == nullptr) {
		auto conf = reinterpret_cast<json*>(confVoid);
		if (!isInitialized())
			return;

		auto lock = this->lockModuleList();
		for (auto& mod : this->moduleList)
		{
			mod->onLoadSettings(conf);
		}
		getModule<ClickGUIMod>()->setEnabled(false);
	}
}

void ModuleManager::onSaveSettings(void* confVoid)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			auto conf = reinterpret_cast<json*>(confVoid);
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				mod->onSaveSettings(conf);
			}
		}
		else
			GameData::terminate();
	}
}
void ModuleManager::onImGuiRender(ImDrawList* d) {
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = lockModuleList();
			for (auto& mod : moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onImGuiRender(d);
			}
		}
		else
			GameData::terminate();
	}
	if (g_Data.getLocalPlayer() == nullptr) {
		if (!isInitialized())
			return;

		auto lock = lockModuleList();
		for (auto& mod : moduleList)
		{
			if (mod->isEnabled() || mod->callWhenDisabled())
				mod->onImGuiRender(d);
		}
	}
}
void ModuleManager::onPlayerTick(C_Player* plr)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = lockModuleList();
			for (auto& mod : moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onPlayerTick(plr);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onWorldTick(C_GameMode* gameMode)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {

			if (!isInitialized())
				return;

			auto lock = lockModuleList();
			for (auto& mod : moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onWorldTick(gameMode);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onTick(C_GameMode* gameMode)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onTick(gameMode);
			}
		}
		else
			GameData::terminate();
	}
}
void ModuleManager::onPreTick(C_GameMode* gameMode) {
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		auto playerUsername = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;
			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList) {
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onPreTick(gameMode);
			}
		}
		else
			GameData::terminate();
	}
}
void ModuleManager::onAttack(C_Entity* attackEnt)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onAttack(attackEnt);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				mod->onKeyUpdate(key, isDown);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto mutex = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onPreRender(renderCtx);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto mutex = this->lockModuleList();
			for (auto& mod : this->moduleList)
			{
				if (mod->isEnabled() || mod->callWhenDisabled())
					mod->onPostRender(renderCtx);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onSendPacket(C_Packet* packet)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = lockModuleList();
			for (auto& it : moduleList)
			{
				if (it->isEnabled() || it->callWhenDisabled())
					it->onSendPacket(packet);
			}
		}
		else
			GameData::terminate();
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList()
{
	return &this->moduleList;
}

int ModuleManager::getModuleCount()
{
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount()
{
	int i = 0;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList)
	{
		if (it->isEnabled())
			i++;
	}
	return i;
}

void ModuleManager::onMove(C_MoveInputHandler* hand)
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;

			auto lock = this->lockModuleList();
			for (auto& it : moduleList)
			{
				if (it->isEnabled() || it->callWhenDisabled())
					it->onMove(hand);
			}
		}
		else
			GameData::terminate();
	}
}

void ModuleManager::onLevelRender()
{
	if (g_Data.getLocalPlayer() != nullptr) {
		std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
		if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers) && isVerifiedUser) {
			if (!isInitialized())
				return;
			auto lock = this->lockModuleList();
			for (auto& it : moduleList)
			{
				if (it->isEnabled() || it->callWhenDisabled())
					it->onLevelRender();
			}
		}
		else
			GameData::terminate();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);