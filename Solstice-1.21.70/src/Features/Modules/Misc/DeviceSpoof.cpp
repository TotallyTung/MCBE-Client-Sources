//
// Created by vastrakai on 7/3/2024.
//

#include "DeviceSpoof.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/ConnectionRequestEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Network/ConnectionRequest.hpp>
#include <SDK/Minecraft/Network/Packets/LoginPacket.hpp>
#include <SDK/SigManager.hpp>
#include <Utils/Buffer.hpp>
#include "EditionFaker.hpp"

static uintptr_t deviceModelAddr;

void DeviceSpoof::onInit()
{
    deviceModelAddr = SigManager::ConnectionRequest_create_DeviceModel;
}

void DeviceSpoof::inject() {
    MemUtils::ReadBytes((void *) deviceModelAddr, originalData, sizeof(originalData));

    MemUtils::NopBytes(deviceModelAddr, 7);
    patchPtr = AllocateBuffer((void *) deviceModelAddr);
    MemUtils::writeBytes((uintptr_t) patchPtr, patch, sizeof(patch));

    auto toOriginalAddrRip4 = MemUtils::GetRelativeAddress((uintptr_t) patchPtr + sizeof(patch) + 1,
                                                           deviceModelAddr + sizeof(originalData));

    MemUtils::writeBytes((uintptr_t) patchPtr + sizeof(patch), "\xE9", 1);
    MemUtils::writeBytes((uintptr_t) patchPtr + sizeof(patch) + 1, &toOriginalAddrRip4, sizeof(int32_t));

    auto newRelRip4 = MemUtils::GetRelativeAddress(deviceModelAddr + 1, (uintptr_t) patchPtr);

    MemUtils::writeBytes(deviceModelAddr, "\xE9", 1);
    MemUtils::writeBytes(deviceModelAddr + 1, &newRelRip4, sizeof(int32_t));
}

void DeviceSpoof::eject()
{
    MemUtils::writeBytes(deviceModelAddr, originalData, sizeof(originalData));
    FreeBuffer(patchPtr);;
}

void DeviceSpoof::spoofMboard() {
    auto editionFaker = gFeatureManager->mModuleManager->getModule<EditionFaker>();

    if(!editionFaker || !editionFaker->mEnabled){
        DeviceModel = StringUtils::generateMboard(7);
    }
    else{
        DeviceModel = StringUtils::generateMboard(editionFaker->mOs.as<int>());
    }

    auto newDeviceModel = reinterpret_cast<uintptr_t>(&DeviceModel);
    MemUtils::writeBytes((uintptr_t) patchPtr + 2, (void *) &newDeviceModel, sizeof(uintptr_t));
}

void DeviceSpoof::onEnable()
{
    inject();
    gFeatureManager->mDispatcher->listen<ConnectionRequestEvent, &DeviceSpoof::onConnectionRequestEvent>(this);
}

void DeviceSpoof::onDisable()
{
    gFeatureManager->mDispatcher->deafen<ConnectionRequestEvent, &DeviceSpoof::onConnectionRequestEvent>(this);
    eject();
}

void DeviceSpoof::onConnectionRequestEvent(ConnectionRequestEvent& event)
{
    auto editionFaker = gFeatureManager->mModuleManager->getModule<EditionFaker>();
    std::string deviceId;

    if(!editionFaker || !editionFaker->mEnabled) {
        deviceId = StringUtils::generateUUID(7);
    }
    else {
        deviceId = StringUtils::generateUUID(editionFaker->mOs.as<int>());
    }

    event.mClientRandomId = StringUtils::generateCID();
    *event.mDeviceId = deviceId;
    *event.mSkinId = "Custom" + deviceId;
    *event.mSelfSignedId = StringUtils::generateUUID(7);

    spoofMboard();
}

/* // Example of a connection request json
{
"AnimatedImageData": [],
"ArmSize": "wide",
"CapeData": "redacted",
"CapeId": "",
"CapeImageHeight": 32,
"CapeImageWidth": 64,
"CapeOnClassicSkin": false,
"ClientRandomId": 24306,
"CompatibleWithClientSideChunkGen": true,
"CurrentInputMode": 1,
"DefaultInputMode": 1,
"DeviceId": "64a116be-2084-4331-a219-288007d1f25c",
"DeviceModel": "System Product Name ASUS",
"DeviceOS": 7,
"GameVersion": "1.21.21",
"GuiScale": -1,
"IsEditorMode": false,
"LanguageCode": "en_US",
"OverrideSkin": false,
"PersonaPieces": [],
"PersonaSkin": false,
"PieceTintColors": [],
"PlatformOfflineId": "",
"PlatformOnlineId": "",
"PlayFabId": "a0b41faabaa2b85a",
"PremiumSkin": true,
"SelfSignedId": "11a764b1-f857-4381-a16e-17a1b2a977b8",
"ServerAddress": ":0",
"SkinAnimationData": "",
"SkinColor": "#0",
"SkinData": "redacted",
"SkinGeometryData": "redacted",
"SkinGeometryDataEngineVersion": "MC4wLjA=",
"SkinId": "Custom64a116be-2084-4331-a219-288007d1f25c",
"SkinImageHeight": 64,
"SkinImageWidth": 64,
"SkinResourcePatch": "ewogICAiYW5pbWF0aW9ucyIgOiB7CiAgICAgICJtb3ZlLmFybXMiIDogImFuaW1hdGlvbi5wbGF5ZXIubW92ZS5hcm1zLnpvbWJpZSIKICAgfSwKICAgImdlb21ldHJ5IiA6IHsKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5Lk1pbmlHYW1lTWFzdGVycy5Xb2xmR2xpZGVyIgogICB9Cn0K",
"ThirdPartyName": "M1tchellPZDC",
"ThirdPartyNameOnly": false,
"TrustedSkin": true,
"UIProfile": 0
}
*/

/*static std::vector<std::string> deviceModels = {
    "Microsoft Surface Pro 9",
    "Dell XPS 13",
    "HP Spectre x360",
    "Lenovo ThinkPad X1 Carbon Gen 11",
    "ASUS ZenBook 14",
    "Acer Swift 5",
    "Razer Blade 15",
    "MSI Creator Z16",
    "HP Elite Dragonfly G3",
    "Dell Latitude 7420",
    "Lenovo Yoga 9i",
    "Microsoft Surface Laptop Studio",
    "ASUS ROG Zephyrus G14",
    "Dell Precision 5570",
    "Lenovo Legion 7i",
    "HP Omen 16",
    "Microsoft Surface Go 3",
    "ASUS ExpertBook B9",
    "Samsung Galaxy Book3 Pro 360",
    "Acer Predator Helios 300"
};

// Can't be used until i figure out how to resign jwt with its private key

void DeviceSpoof::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() != PacketID::Login) return;

    auto loginPacket = event.getPacket<LoginPacket>();

    auto json = loginPacket->mConnectionRequest->toJson();
    spdlog::info("[device spoof] {}", json.dump(4));
    json["DeviceId"] = StringUtils::generateUUID();
    json["SkinId"] = "Custom" + json["DeviceId"].get<std::string>();
    json["SelfSignedId"] = StringUtils::generateUUID();
    json["DeviceModel"] = deviceModels[MathUtils::random(0, deviceModels.size() - 1)];
    loginPacket->mConnectionRequest->fromJson(json);
    loginPacket->mConnectionRequest->generateSignature();
    spdlog::info("[device spoof] successfully spoofed device information [{}]", loginPacket->mConnectionRequest->toString());
}*/