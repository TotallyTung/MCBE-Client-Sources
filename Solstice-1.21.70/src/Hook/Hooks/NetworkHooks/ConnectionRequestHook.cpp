//
// Created by vastrakai on 7/3/2024.
//

#include "ConnectionRequestHook.hpp"
#include <Features/Events/ConnectionRequestEvent.hpp>

std::unique_ptr<Detour> ConnectionRequestHook::mDetour;

void* ConnectionRequestHook::createRequestDetourFunc(ConnectionRequest* result, PrivateKeyManager* privKeyManager,
    Certificate* cert, std::string* selfSignedId, std::string* serverAddress, int64_t clientRandomId,
    std::string* skinId, const char* skinData, const char* capeData, SerializedSkin* skin, std::string* deviceId,
    int inputMode, int uiProfile, int guiScale, std::string* languageCode, bool isEditorMode, bool IsEduMode,
    std::string* tenantId, int8_t adRole, std::string* platformUserId, std::string* thirdPartyName,
    bool thirdPartyNameOnly, std::string* platformOnlineID, std::string* platformOfflineID, std::string* capeId,
    bool CompatibleWithClientSideChunkGen)
{
    auto oFunc = mDetour->getOriginal<&createRequestDetourFunc>();

    auto holder = nes::make_holder<ConnectionRequestEvent>(result, privKeyManager, cert, selfSignedId, serverAddress, clientRandomId,
        skinId, skinData, capeData, skin, deviceId, inputMode, uiProfile, guiScale, languageCode, isEditorMode, IsEduMode,
        tenantId, adRole, platformUserId, thirdPartyName, thirdPartyNameOnly, platformOnlineID, platformOfflineID, capeId,
        CompatibleWithClientSideChunkGen);
    gFeatureManager->mDispatcher->trigger(holder);

    // Uhhhhh what the fhauck am i doing here lol
    result = holder->mResult;
    privKeyManager = holder->mPrivKeyManager;
    cert = holder->mCert;
    *selfSignedId = *holder->mSelfSignedId;
    *serverAddress = *holder->mServerAddress;
    clientRandomId = holder->mClientRandomId;
    *skinId = *holder->mSkinId;
    *deviceId = *holder->mDeviceId;
    inputMode = holder->mInputMode;
    uiProfile = holder->mUiProfile;
    guiScale = holder->mGuiScale;
    *languageCode = *holder->mLanguageCode;
    isEditorMode = holder->mIsEditorMode;
    IsEduMode = holder->mIsEduMode;
    *tenantId = *holder->mTenantId;
    adRole = holder->mAdRole;
    *platformUserId = *holder->mPlatformUserId;
    *thirdPartyName = *holder->mThirdPartyName;
    thirdPartyNameOnly = holder->mThirdPartyNameOnly;
    *platformOnlineID = *holder->mPlatformOnlineID;
    *platformOfflineID = *holder->mPlatformOfflineID;
    *capeId = *holder->mCapeId;
    CompatibleWithClientSideChunkGen = holder->mCompatibleWithClientSideChunkGen;

    return oFunc(result, privKeyManager, cert, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, skin,
        deviceId, inputMode, uiProfile, guiScale, languageCode, isEditorMode, IsEduMode, tenantId, adRole, platformUserId,
        thirdPartyName, thirdPartyNameOnly, platformOnlineID, platformOfflineID, capeId, CompatibleWithClientSideChunkGen);
}

void ConnectionRequestHook::init()
{
    mDetour = std::make_unique<Detour>("ConnectionRequest::create", reinterpret_cast<void*>(SigManager::ConnectionRequest_create), reinterpret_cast<void*>(&createRequestDetourFunc));
}