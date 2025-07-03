#pragma once

void* onConnectionRequestCreate;

void* ConnectionRequestCreateDetour(ConnectionRequest* _this, PrivateKeyManager* a2, Certificate* a3, std::string* a4, std::string* a5, __int64 a6, __int64 a7, __int64 a8, __int64 a9, SerializedSkin* a10, std::string* a11, int a12, int a13, __int64 a14, char a15, char a16, void* a17, int a18, __int64 a19, __int64 a20, char a21, __int64 a22, __int64 a23, __int64 a24, char a25, __int64 a26) {

	const auto& skin = ((Player*)Address::getLocalPlayer())->getSkin();

	if (skin != nullptr) {

	}

	/*std::string path = FileUtils::getResourcePath() + "\\TargetHead.png";

	FILE* fp;
	errno_t err = fopen_s(&fp, path.c_str(), "wb");
	if (fp) {
		svpng(fp, Skin->mSkinImage.mWidth, Skin->mSkinImage.mHeight, (const uint8_t*)SkinData, 1);
		fclose(fp);
	}*/

	std::string serverAddr = *a5;
	// if the server address contains a colon, then split it and use the first part
	if (serverAddr.find(":") != std::string::npos) {
		serverAddr = serverAddr.substr(0, serverAddr.find(':'));
	}

	Global::Notification.addNotification("Floence", "Connecting to " + serverAddr + "...", 3.f);

	if (getModuleByName("devicespoofer")->isEnabled()) {
		std::string generated = "";
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint64_t> dis64;
		std::uniform_int_distribution<uint32_t> dis32;

		static auto to_hex = [](uint64_t value, int width) {
			return std::format("{:0{}x}", value, width);
			};

		generated =
			to_hex(dis32(gen), 8) + "-" +
			to_hex(dis32(gen) & 0xFFFF, 4) + "-" +
			to_hex((dis32(gen) & 0x0FFF) | 0x4000, 4) + "-" +
			to_hex((dis32(gen) & 0x3FFF) | 0x8000, 4) + "-" +
			to_hex(dis64(gen), 12).substr(0, 12);

		*a11 = generated;

		Global::Notification.addNotification("Floence", "Spoofed your device id", 3.f);
	}

	return Memory::CallFunc<void*, ConnectionRequest*, PrivateKeyManager*, Certificate*, std::string*, std::string*, __int64, __int64, __int64, __int64, SerializedSkin*, std::string*, int, int, __int64, char, char, void*, int, __int64, __int64, char, __int64, __int64, __int64, char, __int64>(onConnectionRequestCreate, _this, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
}

class ConnectionRequestHook : public FuncHook {
public:
	bool Initialize() override {
		// ConnectionRequest::create
		void* connectionRequestCreateAddr = Memory::findSig(xorstr_("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B D9 48 89 54 24"));

		if (!Memory::HookFunction(connectionRequestCreateAddr, (void*)&ConnectionRequestCreateDetour, &onConnectionRequestCreate, xorstr_("ConnectionRequestCreate"))) { return false; }

		return true;
	}

	static ConnectionRequestHook& Instance() {
		static ConnectionRequestHook instance;
		return instance;
	}
};