#pragma once
//
// Created by vastrakai on 9/17/2023.
//


#include <Prax/Hook/Hook.h>

class Social : public Hook {
public:
    Social() : Hook("Social", true) {};

    // last join time
    static uint64_t lastJoinTime;

    static void* createRequestDetourFunc(class ConnectionRequest* result, class PrivateKeyManager* privKeyManager,
                                         class Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
                                         std::string* skinId, const char* skinData, const char* capeData, class SerializedSkin* skin,
                                         std::string* deviceId, int inputMode, int uiProfile, int guiScale, std::string* languageCode,
                                         bool isEditorMode, bool IsEduMode, std::string* tenantId, int8_t adRole, std::string* platformUserId,
                                         std::string* thirdPartyName, bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID,
                                         std::string* capeId, bool CompatibleWithClientSideChunkGen);
    void Init() override;
    void UnHook() override;
};