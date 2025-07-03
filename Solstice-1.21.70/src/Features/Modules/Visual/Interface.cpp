//
// Created by vastrakai on 7/1/2024.
//

#include <Features/Events/ActorRenderEvent.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/DrawImageEvent.hpp>
#include <Features/Events/ModuleStateChangeEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/DrawImageEvent.hpp>
#include <Features/Events/PreGameCheckEvent.hpp>
#include <Features/Events/RenderEvent.hpp>

#include <Features/Modules/Visual/Interface.hpp>
#include <Hook/Hooks/RenderHooks/ActorRenderDispatcherHook.hpp>
#include <Hook/Hooks/RenderHooks/HoverTextRendererHook.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/mce.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

#include <SDK/Minecraft/Actor/SyncedPlayerMovementSettings.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/World/Level.hpp>

#include "ClickGui.hpp"

#ifdef __DEBUG__
std::vector<unsigned char> gFsBytes2 = { 0x0f, 0x85 };
DEFINE_PATCH_FUNC(patchFullStack, SigManager::ResourcePackManager_composeFullStackBp, gFsBytes2);
#endif

// please someone make this in a class or struct cuz it gives me aids
// Define a mapping from Minecraft color codes to RGBA colors
std::unordered_map<char, ImColor> mColorMap = {
    {'0', ImColor(0, 0, 0)},        // Black
    {'1', ImColor(0, 0, 170)},      // Dark Blue
    {'2', ImColor(0, 170, 0)},      // Dark Green
    {'3', ImColor(0, 170, 170)},    // Dark Aqua
    {'4', ImColor(170, 0, 0)},      // Dark Red
    {'5', ImColor(170, 0, 170)},    // Dark Purple
    {'6', ImColor(255, 170, 0)},    // Gold
    {'7', ImColor(170, 170, 170)},  // Gray
    {'8', ImColor(85, 85, 85)},     // Dark Gray
    {'9', ImColor(85, 85, 255)},    // Blue
    {'a', ImColor(85, 255, 85)},    // Green
    {'b', ImColor(85, 255, 255)},   // Aqua
    {'c', ImColor(255, 85, 85)},    // Red
    {'d', ImColor(255, 85, 255)},   // Light Purple
    {'e', ImColor(255, 255, 85)},   // Yellow
    {'f', ImColor(255, 255, 255)},  // White
    {'r', ImColor(255, 255, 255)}   // Reset
};

template <typename T>
std::string combine(T t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename T, typename... Args>
std::string combine(T t, Args... args)
{
    std::stringstream ss;
    ss << t << combine(args...);
    return ss.str();
}

float pYaw;
float pOldYaw;

float pHeadYaw;
float pOldHeadYaw;

float pPitch;
float pOldPitch;

float pBodyYaw;
float pOldBodyYaw;

float pLerpedYaw;
float pLerpedHeadYaw;
float pLerpedPitch;
float pLerpedBodyYaw;

bool usingPaip = false;

void Interface::onEnable()
{

}

void Interface::onDisable()
{
#ifdef __DEBUG__
    patchFullStack(false);
#endif
}

void Interface::renderHoverText()
{
    static EasingUtil inEase;

    (HoverTextRender::mTimeDisplayed != 0 && gFeatureManager->mModuleManager->getModule<ClickGui>()->mEnabled != true) ?
            inEase.incrementPercentage(ImRenderUtils::getDeltaTime() * 2)
            : inEase.decrementPercentage(ImRenderUtils::getDeltaTime() * 4);

    float inScale = HoverTextRender::mTimeDisplayed != 0 && gFeatureManager->mModuleManager->getModule<ClickGui>()->mEnabled != true ? inEase.easeOutExpo() : inEase.easeOutBack();

    if (inEase.isPercentageMax())
        inScale = 1;

    if (inScale < 0.01)
        return;

    glm::vec2 mPos = HoverTextRender::mInfo.mPos;
    glm::vec2 mTextPos = glm::vec2(mPos.x + 6, mPos.y + 6); // It looks better this way than getting it from HoverTextRenderer class

    float mTextSize = 1.25 * inScale;

    std::string mMessage = HoverTextRender::mInfo.mText;
    std::string mNoneColoredText = ColorUtils::removeColorCodes(HoverTextRender::mInfo.mText);

    ImColor mCurrentColor = ImColor(255, 255, 255);

    float mMeasurementX = ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, -1, mNoneColoredText.c_str()).x;
    float mMeasurementY = ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, -1, mNoneColoredText.c_str()).y;

    ImVec4 mRect = ImVec4(mPos.x, mPos.y, mPos.x + mMeasurementX + 12, mPos.y + mMeasurementY + 12);

    ImRenderUtils::addBlur(mRect, 3 * inScale, 10);

    ImRenderUtils::fillRectangle(mRect, ImColor(0, 0, 0), 0.78f * inScale, 10);

    for (size_t j = 0; j < mMessage.length(); ++j) {
        char c = mMessage[j];

        if (c == '§' && j + 1 < mMessage.length()) {
            char colorCode = mMessage[j + 1];
            if (mColorMap.find(colorCode) != mColorMap.end()) {
                mCurrentColor = mColorMap[colorCode];
                j++;
            }
            continue;
        }

        if (c == '\n') {
            mTextPos.x = mPos.x + 6;
            mTextPos.y += ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, 0, "\n").y;
        }

        if (!std::isprint(c)) {
            continue;
        }

        std::string mString = combine(c, "");

        ImRenderUtils::drawText(mTextPos, mString, mCurrentColor, mTextSize, inScale, false);

        mTextPos.x += ImGui::GetFont()->CalcTextSizeA(mTextSize * 18, FLT_MAX, -1, mString.c_str()).x;
    }

    HoverTextRender::mTimeDisplayed = 0;
}


void Interface::onModuleStateChange(ModuleStateChangeEvent& event)
{
    if (event.mModule == this)
    {
        event.setCancelled(true);
    }
}

void Interface::onPregameCheckEvent(PreGameCheckEvent& event)
{
#ifdef __DEBUG__
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || ! mForcePackSwitching.mValue) return;

    std::string screenName = ClientInstance::get()->getScreenName();

    // prevent other screens from breaking
    if (screenName.contains("screen_world_controls_and_settings") && !screenName.contains("global_texture_pack_tab")) return;

    event.setPreGame(true);
#endif
}

void Interface::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    static bool lastPlayerState = false;

    //renderHoverText();

#ifdef __DEBUG__
    if (player && mForcePackSwitching.mValue)
    {
        patchFullStack(true);
    } else
    {
        patchFullStack(false);
    }
#endif

    if (player && !lastPlayerState)
    {
        usingPaip = false;
    }

    static constexpr float LERP_SPEED = 20.f;
    float deltaTime = ImGui::GetIO().DeltaTime;

    float yaw = MathUtils::wrap(pLerpedYaw, pYaw - 180, pYaw + 180);
    float headYaw = MathUtils::wrap(pLerpedHeadYaw, pHeadYaw - 180, pHeadYaw + 180);
    float pitch = pLerpedPitch;
    float bodyYaw = MathUtils::wrap(pLerpedBodyYaw, pBodyYaw - 180, pBodyYaw + 180);

    float preLerpedYaw = MathUtils::lerp(yaw, pYaw, deltaTime * LERP_SPEED);
    float preLerpedHeadYaw = MathUtils::lerp(headYaw, pHeadYaw, deltaTime * LERP_SPEED);
    float preLerpedPitch = MathUtils::lerp(pitch, pPitch, deltaTime * LERP_SPEED);
    float preLerpedBodyYaw = MathUtils::lerp(bodyYaw, pBodyYaw, deltaTime * LERP_SPEED);

    pLerpedYaw = MathUtils::wrap(pLerpedYaw, preLerpedYaw - 180, preLerpedYaw + 180);
    pLerpedHeadYaw = MathUtils::wrap(pLerpedHeadYaw, preLerpedHeadYaw - 180, preLerpedHeadYaw + 180);
    pLerpedBodyYaw = MathUtils::wrap(pLerpedBodyYaw, preLerpedBodyYaw - 180, preLerpedBodyYaw + 180);

    pLerpedYaw = MathUtils::lerp(preLerpedYaw, pLerpedYaw, deltaTime * LERP_SPEED);
    pLerpedHeadYaw = MathUtils::lerp(preLerpedHeadYaw, pLerpedHeadYaw, deltaTime * LERP_SPEED);
    pLerpedPitch = MathUtils::lerp(preLerpedPitch, pLerpedPitch, deltaTime * LERP_SPEED);
    pLerpedBodyYaw = MathUtils::lerp(preLerpedBodyYaw, pLerpedBodyYaw, deltaTime * LERP_SPEED);

}

void Interface::onActorRenderEvent(ActorRenderEvent& event)
{
    if (event.isCancelled()) return;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    
    if (event.mEntity != player) return;
    if (*event.mPos == glm::vec3(0.f, 0.f, 0.f) && *event.mRot == glm::vec2(0.f, 0.f)) return;

    bool firstPerson = ClientInstance::get()->getOptions()->mThirdPerson->value == 0;
    if (firstPerson && !player->getFlag<RenderCameraComponent>()) return;


    const auto actorRotations = event.mEntity->getActorRotationComponent();
    const auto headRotations = event.mEntity->getActorHeadRotationComponent();
    const auto bodyRotations = event.mEntity->getMobBodyRotationComponent();
    if (!actorRotations || !headRotations || !bodyRotations) return;

    float realOldPitch = actorRotations->mOldPitch;
    float realPitch = actorRotations->mPitch;
    float realHeadRot = headRotations->mHeadRot;
    float realOldHeadRot = headRotations->mOldHeadRot;
    float realBodyYaw = bodyRotations->yBodyRot;
    float realOldBodyYaw = bodyRotations->yOldBodyRot;

    actorRotations->mOldPitch = pLerpedPitch;
    actorRotations->mPitch = pLerpedPitch;
    headRotations->mHeadRot = pLerpedHeadYaw;
    headRotations->mOldHeadRot = pLerpedHeadYaw;
    bodyRotations->yBodyRot = pLerpedBodyYaw;
    bodyRotations->yOldBodyRot = pLerpedBodyYaw;

    auto original = event.mDetour->getOriginal<&ActorRenderDispatcherHook::render>();
    original(event._this, event.mEntityRenderContext, event.mEntity, event.mCameraTargetPos, event.mPos, event.mRot, event.mIgnoreLighting);
    event.cancel();

    actorRotations->mOldPitch = realOldPitch;
    actorRotations->mPitch = realPitch;
    headRotations->mHeadRot = realHeadRot;
    headRotations->mOldHeadRot = realOldHeadRot;
    bodyRotations->yBodyRot = realBodyYaw;
    bodyRotations->yOldBodyRot = realOldBodyYaw;
}

/*void Interface::onDrawImageEvent(DrawImageEvent& event)
{
    if (!mSlotEasing.mValue) return;
    // Wait for ImGui to be initialized
    if (!ImGui::GetCurrentContext()) return;

    static glm::vec2 hotbarPos = {};
    auto path = event.mTexture->mTexture->mFilePath.c_str();

    // textures/ui/selected_hotbar_slot
    // If the selected hotbar slot is being drawn
    if (strcmp(path, "textures/ui/selected_hotbar_slot") == 0)
    {
        float deltaTime = ImGui::GetIO().DeltaTime;
        if (hotbarPos.x == 0 || hotbarPos.y == 0) hotbarPos = *event.mPos;
        hotbarPos.x = MathUtils::lerp(hotbarPos.x, event.mPos->x, deltaTime * mSlotEasingSpeed.mValue);
        hotbarPos.y = event.mPos->y;
        *event.mPos = hotbarPos;
    }
}*/


void Interface::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();

    BodyYaw::updateRenderAngles(player, pYaw);
    pOldBodyYaw = pBodyYaw;
    pBodyYaw = BodyYaw::bodyYaw;
}

void Interface::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput) usingPaip = true;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    auto level = player->getLevel();
    if (!level) return;
    auto moveSettings = level->getPlayerMovementSettings();
    if (!moveSettings) return;
    bool isServerAuthoritative = usingPaip;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput && isServerAuthoritative)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        pOldYaw = pYaw;
        pOldPitch = pPitch;
        pOldBodyYaw = pBodyYaw;
        pYaw = paip->mRot.y;
        pPitch = paip->mRot.x;
        pHeadYaw = paip->mYHeadRot;
    }
    else if (event.mPacket->getId() == PacketID::MovePlayer && !isServerAuthoritative)
    {
        auto mpp = event.getPacket<MovePlayerPacket>();
        pOldYaw = pYaw;
        pOldPitch = pPitch;
        pOldBodyYaw = pBodyYaw;
        pYaw = mpp->mRot.y;
        pPitch = mpp->mRot.x;
        pHeadYaw = mpp->mYHeadRot;
    }
}
