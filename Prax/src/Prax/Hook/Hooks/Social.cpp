//
// Created by vastrakai on 9/17/2023.
//

#include <memory>
#include <Prax/SDK/SigManager.h>
#include <Prax/Utils/String.h>
#include <Prax/UI/Notifications.h>
#include <Prax/Module/Modules/Misc/DeviceIDSpoof.h>
#include <Prax/Module/ModuleManager.h>
#include "Social.h"

std::unique_ptr<Detour> createRequestDetour = nullptr;
std::unique_ptr<Detour> getDeviceIdDetour = nullptr;

DeviceIDSpoof* deviceIDSpoof = nullptr;

uint64_t Social::lastJoinTime = Misc::GetCurrentMs();


void* Social::createRequestDetourFunc(class ConnectionRequest* result, class PrivateKeyManager* privKeyManager,
                          class Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
                          std::string* skinId, const char* skinData, const char* capeData, class SerializedSkin* skin,
                          std::string* deviceId, int inputMode, int uiProfile, int guiScale, std::string* languageCode,
                          bool isEditorMode, bool IsEduMode, std::string* tenantId, int8_t adRole, std::string* platformUserId,
                          std::string* thirdPartyName, bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID,
                          std::string* capeId, bool CompatibleWithClientSideChunkGen) {
    auto oFunc = createRequestDetour->GetFastcall<void*, class ConnectionRequest*, class PrivateKeyManager*,
            class Certificate*, std::string*, std::string*, int64_t, std::string*, const char*, const char*, class SerializedSkin*,
            std::string*, int, int, int, std::string*, bool, bool, std::string*, int8_t, std::string*, std::string*, bool,
            std::string*, std::string*, std::string*, bool>();


    if (!deviceIDSpoof) deviceIDSpoof = ModuleManager::GetModule<DeviceIDSpoof>();


    if (deviceIDSpoof && deviceIDSpoof->Enabled) {

        *deviceId = DeviceIDSpoof::DeviceID;
        clientRandomId = ((int64_t) rand() << 32) | rand();
        *selfSignedId = String::GenerateUUID();
        *skinId = "Custom" + DeviceIDSpoof::DeviceID;

        Logger::Write("ConnectionRequest_create", "Spoofed device id (" + *deviceId + "), client random id (" + std::to_string(clientRandomId) + "), and self signed id (" + *selfSignedId + ")");
    }

    lastJoinTime = Misc::GetCurrentMs();



    std::string serverAddr = *serverAddress;
    // if the server address contains a colon, then split it and use the first part
    if (serverAddr.contains(':')) {
        serverAddr = serverAddr.substr(0, serverAddr.find(':'));
    }

    Notifications::Notify(Prax::ClientName, "Connecting to " + serverAddr + "...", 3.f);

    return oFunc(result, privKeyManager, cert, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, skin,
                     deviceId, inputMode, uiProfile, guiScale, languageCode, isEditorMode, IsEduMode, tenantId, adRole, platformUserId,
                     thirdPartyName, thirdPartyNameOnly, platformOnlineID, platformOfflineID, capeId, CompatibleWithClientSideChunkGen);
}


std::string* deviceIdHook(void* _this, void* a2){
    auto oFunc = getDeviceIdDetour->GetFastcall<std::string*, void*, void*>();

    if (!deviceIDSpoof) deviceIDSpoof = ModuleManager::GetModule<DeviceIDSpoof>();

    std::string* result = oFunc(_this, a2);
    if (deviceIDSpoof && deviceIDSpoof->Enabled) {
        *result = DeviceIDSpoof::DeviceID;
    }

    return result;
}


void Social::Init() {
    String::GenerateUUID();
    uintptr_t createRequestAddr = SigManager::GetSignatureAddress(SignatureEnum::ConnectionRequest_create);
    createRequestDetour = std::make_unique<Detour>("createRequestDetour", createRequestAddr, &createRequestDetourFunc);
    createRequestDetour->Enable();

    uintptr_t getDeviceIdAddr = SigManager::GetSignatureAddress(SignatureEnum::AppPlatform_getDeviceId);
    getDeviceIdDetour = std::make_unique<Detour>("getDeviceIdDetour", getDeviceIdAddr, &deviceIdHook);
    getDeviceIdDetour->Enable();

    this->Detours.emplace_back(createRequestDetour.get());
    this->Detours.emplace_back(getDeviceIdDetour.get());
}

void Social::UnHook() {

}

