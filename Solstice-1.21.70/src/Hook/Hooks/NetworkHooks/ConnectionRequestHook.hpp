#pragma once
//
// Created by vastrakai on 7/3/2024.
//

#include <Hook/Hook.hpp>


/*
class ConnectionRequest* result, class PrivateKeyManager* privKeyManager,
                          class Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
                          std::string* skinId, const char* skinData, const char* capeData, class SerializedSkin* skin,
                          std::string* deviceId, int inputMode, int uiProfile, int guiScale, std::string* languageCode,
                          bool isEditorMode, bool IsEduMode, std::string* tenantId, int8_t adRole, std::string* platformUserId,
                          std::string* thirdPartyName, bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID,
                          std::string* capeId, bool CompatibleWithClientSideChunkGen*/

class ConnectionRequestHook : public Hook {
public:
    ConnectionRequestHook() : Hook()
    {
        mName = "ConnectionRequest::create";
    };

    static std::unique_ptr<Detour> mDetour;

    static void* createRequestDetourFunc(class ConnectionRequest* result, class PrivateKeyManager* privKeyManager,
                                         class Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
                                         std::string* skinId, const char* skinData, const char* capeData, class SerializedSkin* skin,
                                         std::string* deviceId, int inputMode, int uiProfile, int guiScale, std::string* languageCode,
                                         bool isEditorMode, bool IsEduMode, std::string* tenantId, int8_t adRole, std::string* platformUserId,
                                         std::string* thirdPartyName, bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID,
                                         std::string* capeId, bool CompatibleWithClientSideChunkGen);
    void init() override;
};

