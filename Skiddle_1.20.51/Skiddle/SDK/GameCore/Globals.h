#pragma once

class FontRepos;

#include "InspectMenu.h"

// GameCore SDK
namespace Game
{
	namespace RenderInfo
	{
		static inline HWND window = nullptr;
		static inline Vector2<float> windowSize = Vector2<float>{};
	}

	namespace TargetLists
	{
		std::vector<Player*> auraList = std::vector<Player*>();
		std::vector<Player*> targethudList = std::vector<Player*>();
		std::vector<Player*> hitsoundsList = std::vector<Player*>();
		std::vector<Player*> infAuraList = std::vector<Player*>();
	}

	class Skiddle
	{
	public:
		static inline std::string ClientName = "Skiddle";
		static inline int ClientColor = 11;

		static inline bool showNotification = false;
		static inline bool showArrayMode = true;

		static inline bool ShouldBlock = false;
		static inline bool skipTicks = false;
	};

	class FontRepos
	{
	public:
		static inline std::vector<FontBitmap*> fonts = {};

		static const char* FontName(FontBitmap* font)
		{
			if (font->FontPath[0] == 'f')
				return "Mojangles";

			return "Unknown";
		}

		static void PutFont(FontBitmap* font)
		{
			bool fontExists = false;
			for (FontBitmap* fontPtr : fonts)
			{
				if (font == fontPtr)
					fontExists = true;
			}

			if (not fontExists)
			{
				fonts.push_back(font);
			}
		}

		static FontBitmap* GetFont(const char* font)
		{
			for (FontBitmap* fontPtr : fonts)
			{
				if (strcmp(FontName(fontPtr), font) == 0)
					return fontPtr;
			}

			return nullptr;
		}

		static FontBitmap* GetClientFont()
		{
			return GetFont("Mojangles");
		}
	};

	class Core
	{
	public:
		static inline NotificationBase noti{};

		static inline Player* Player = nullptr;
		static inline Mob* Mob = nullptr;
		static inline ClientInstance* ClientInstance = nullptr;
		static inline GameMode* GameMode = nullptr;

		static inline bool InChat = false;
		static inline bool HudScreen = false;

		static inline std::map<__int32, bool> Keymap = {};
		static inline std::string User = "Unknown";

		static inline float HotbarSpeed = 15.f;
		static inline float SwingSpeed = 6;
		static inline float AnimationSwingSpeed = 6;
		static inline float FieldMultiplier = 1.1f;
		static inline bool ApplyField = false;

		static inline float headYaw = 0.f;
		static inline float bodyYaw = 0.f;
		static inline float pitch = 0.f;


		static inline float easedHeadYaw = 0.f;
		static inline float easedBodyYaw = 0.f;
		static inline float easedPitch = 0.f;

		static inline float gamma = 100;
		static inline bool togglesound = false;
		static inline int soundtype = 0;

		static inline bool isLeftClickDown = false;
		static inline bool isRightClickDown = false;

		static inline bool showMenuBackground = true;
		static inline bool showAntiPiracyScreen1 = false;
		static inline bool showMenu = true;

	public:
		static bool IsInChat()
		{
			return InChat;
		}

		static inline void SetFieldMulti(float value)
		{
			ApplyField = true;
			FieldMultiplier = value;
		}

		static inline void ResetFieldMulti()
		{
			ApplyField = false;
		}
	};

	static void Setup(ClientInstance* instance, float deltaTime, Vector2<float> screenScale)
	{
		Core::ClientInstance = instance;

		GuiInfo::DeltaTime = deltaTime;

		GuiInfo::GuiScale = instance->getGuiData()->GuiScale;

		GuiInfo::TrueScreenRes = screenScale / instance->getGuiData()->GuiScale;
		GuiInfo::ScreenRes = screenScale;

		GuiInfo::MousePos = instance->getGuiData()->MousePos / instance->getGuiData()->GuiScale;
		GuiInfo::TrueMousePos = instance->getGuiData()->MousePos;

		Core::Player = instance->getLocalPlayer();
	}

	Player* GetLocalPlayer() { return Core::Player; } // so people dont WINGE about it
	Mob* getMob() { return Core::Mob; } // so people dont WINGE about it
	GameMode* GetGameMode() { return Core::GameMode; }
	ClientInstance* GetInstance() { return Core::ClientInstance; }

	char currentScreenName[100];

	static int gameFps;
	static int frameCount;
	static int cps;
	static int cpsCount;

	bool killed = false;
	int kills = 0;
	bool sendQueue = false;
	bool sendGG = false;
	bool startleaks = false;
	bool detectedleaks = false;

	InspectMenu* GetInspecter() // dis so BAD BRO
	{
		static uintptr_t offset = GetXRef(findOffset("80 3D ? ? ? ? 00 ? ? ? ? ? ? 4D 8B 47 48", "Inspector"));

		return new InspectMenu(offset - 4);
	}
}