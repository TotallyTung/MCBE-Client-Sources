//
// Created by vastrakai on 7/3/2024.
//

#pragma once

#include "Event.hpp"

/*
PrivateKeyManager* privKeyManager,
    Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
    std::string* skinId, const char* skinData, const char* capeData, SerializedSkin* skin, std::string* deviceId,
    int inputMode, int uiProfile, int guiScale, std::string* languageCode, bool isEditorMode, bool IsEduMode,
    std::string* tenantId, int8_t adRole, std::string* platformUserId, std::string* thirdPartyName,
    bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID, std::string* capeId,
    bool compatibleWithClientSideChunkGen*/

class ConnectionRequestEvent : public Event
{
public:
    class ConnectionRequest* mResult;
    class PrivateKeyManager* mPrivKeyManager;
    class Certificate* mCert;
    std::string* mSelfSignedId;
    std::string* mServerAddress;
    int64_t mClientRandomId;
    std::string* mSkinId;
    const char* mSkinData;
    const char* mCapeData;
    class SerializedSkin* mSkin;
    std::string* mDeviceId;
    int mInputMode;
    int mUiProfile;
    int mGuiScale;
    std::string* mLanguageCode;
    bool mIsEditorMode;
    bool mIsEduMode;
    std::string* mTenantId;
    int8_t mAdRole;
    std::string* mPlatformUserId;
    std::string* mThirdPartyName;
    bool mThirdPartyNameOnly;
    std::string* mPlatformOnlineID;
    std::string* mPlatformOfflineID;
    std::string* mCapeId;
    bool mCompatibleWithClientSideChunkGen;

    explicit ConnectionRequestEvent(class ConnectionRequest* result, class PrivateKeyManager* privKeyManager,
                                    class Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
                                    std::string* skinId, const char* skinData, const char* capeData, class SerializedSkin* skin,
                                    std::string* deviceId, int inputMode, int uiProfile, int guiScale, std::string* languageCode,
                                    bool isEditorMode, bool IsEduMode, std::string* tenantId, int8_t adRole, std::string* platformUserId,
                                    std::string* thirdPartyName, bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID,
                                    std::string* capeId, bool compatibleWithClientSideChunkGen) :
            mResult(result), mPrivKeyManager(privKeyManager), mCert(cert), mSelfSignedId(selfSignedId), mServerAddress(serverAddress),
            mClientRandomId(clientRandomId), mSkinId(skinId), mSkinData(skinData), mCapeData(capeData), mSkin(skin), mDeviceId(deviceId),
            mInputMode(inputMode), mUiProfile(uiProfile), mGuiScale(guiScale), mLanguageCode(languageCode), mIsEditorMode(isEditorMode),
            mIsEduMode(IsEduMode), mTenantId(tenantId), mAdRole(adRole), mPlatformUserId(platformUserId), mThirdPartyName(thirdPartyName),
            mThirdPartyNameOnly(thirdPartyNameOnly), mPlatformOnlineID(platformOnlineID), mPlatformOfflineID(platformOfflineID),
            mCapeId(capeId), mCompatibleWithClientSideChunkGen(compatibleWithClientSideChunkGen) {};
};



