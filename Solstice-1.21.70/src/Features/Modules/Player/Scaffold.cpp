//
// Created by vastrakai on 7/10/2024.
//

#include "Scaffold.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Modules/Combat/Aura.hpp>
#include <Features/Modules/Misc/TestModule.hpp>
#include <Features/Modules/Visual/Interface.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <Features/Modules/Visual/Interface.hpp>
#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/KeyboardMouseSettings.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>

void Scaffold::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Scaffold::onBaseTickEvent, nes::event_priority::LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Scaffold::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);


    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mStartY = player->getPos()->y - PLAYER_HEIGHT - 1.f;
    mLastSlot = player->getSupplies()->mSelectedSlot;
}

void Scaffold::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Scaffold::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Scaffold::onPacketOutEvent>(this);

    // Reset fields
    mStartY = 0.f;
    mLastBlock = {0, 0, 0};
    mLastFace = 0;
    mLastSwitchTime = 0;
    mShouldRotate = false;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    if (mLastSlot != -1)
    {
        player->getSupplies()->mSelectedSlot = mLastSlot;
    }

    if (mIsTowering)
    {
        mIsTowering = false;
        if (mTowerMode.as<TowerMode>() != TowerMode::Vanilla)
        {
            player->getStateVectorComponent()->mVelocity.y = -5.0f;
            ClientInstance::get()->getMinecraftSim()->setSimTimer(20.0f);
        }
    }
}



void Scaffold::onBaseTickEvent(BaseTickEvent& event)
{
    mShouldClip = false;
    auto player = event.mActor;

    int places = mPlaces.as<int>();

    if (mFastClutch.mValue && player->getFallDistance() > mClutchFallDistance.mValue)
    {
        places = mCluchPlaces.as<int>();
    }

    for (int i = 0; i < places; i++)
    {
        if (!tickPlace(event)) break;
        if (mShouldClip) break;
    }
}

bool Scaffold::tickPlace(BaseTickEvent& event)
{
    auto player = event.mActor;

    // components
    auto moveInput = player->getMoveInputComponent();
    auto actorRot = player->getActorRotationComponent();
    auto stateVec = player->getStateVectorComponent();


    auto currentY = player->getPos()->y - 2.62f;
    if (!mLockY.mValue) mStartY = currentY;
    if (player->getPos()->y - 2.62f < mStartY) mStartY = player->getPos()->y - 2.62f;
    // If space is held unlock Y
    if (moveInput->mIsJumping && !Keyboard::isUsingMoveKeys()) mStartY = currentY;
    float yaw = actorRot ->mYaw + MathUtils::getRotationKeyOffset() + 90;

    glm::vec3 velocity = stateVec ->mVelocity;

    bool isMoving = Keyboard::isUsingMoveKeys();

    float maxExtend = mExtend.mValue;

    if (ItemUtils::getAllPlaceables(mHotbarOnly.mValue) == 0)
    {
        if (mIsTowering)
        {
            mIsTowering = false;
            stateVec ->mVelocity.y = -5.0f;
        }
        return false;
    }

    if (mSwitchMode.mValue == SwitchMode::Fake && mLastSlot != -1) player->getSupplies()->mInHandSlot = mLastSlot; // Change the

    if (mPlacementMode.mValue == PlacementMode::Flareon)
    {
        yaw = MathUtils::snapYaw(yaw);
    }

    glm::vec3 blockPos = getPlacePos(0.f);

    if (!Keyboard::isUsingMoveKeys())
    {
        maxExtend = 0.f;
    }

    //bool space = Keyboard::mPressedKeys[VK_SPACE];
    auto& keyboard = *ClientInstance::get()->getKeyboardSettings();
    bool space = Keyboard::mPressedKeys[keyboard["key.jump"]];
    bool wasTowering = mIsTowering;


    float fallDistance = player->getFallDistance();
    if (!mFallDistanceCheck.mValue) fallDistance = 0.f;
    switch (mTowerMode.mValue)
    {
    default:
        break;
    case TowerMode::Velocity:
        {
            if (ClientInstance::get()->getMouseGrabbed()) break;
            if ((space && mAllowMovement.mValue || space && !isMoving) && fallDistance < 3.f)
            {
                if (!mAllowMovement.mValue)
                {
                    stateVec ->mVelocity.x = 0;
                    stateVec ->mVelocity.z = 0;
                } else if (!player->isOnGround())
                {
                    glm::vec2 currentMotion = {stateVec ->mVelocity.x, stateVec ->mVelocity.z};
                    float movementSpeed = sqrt(currentMotion.x * currentMotion.x + currentMotion.y * currentMotion.y);
                    float movementYaw = atan2(currentMotion.y, currentMotion.x);
                    float moveYawDeg = movementYaw * (180 / IM_PI) - 90.f;
                    float playerYawDeg = actorRot ->mYaw + MathUtils::getRotationKeyOffset();
                    float yawDiff = playerYawDeg - moveYawDeg;
                    float yawDiffRad = yawDiff * (IM_PI / 180);
                    float newMoveYaw = movementYaw + yawDiffRad;
                    stateVec ->mVelocity.x = cos(newMoveYaw) * movementSpeed;;
                    stateVec ->mVelocity.z = sin(newMoveYaw) * movementSpeed;
                }
                mStartY = player->getPos()->y;
                mIsTowering = true;
                stateVec ->mVelocity.y = mTowerSpeed.mValue / 10;
                maxExtend = 0.f;
            }
            else if (wasTowering)
            {
                mIsTowering = false;
                stateVec ->mVelocity.y = -5.0f;
            }
            break;
        }
    case TowerMode::Clip: {
            if (ClientInstance::get()->getMouseGrabbed()) break;
            if ((space && mAllowMovement.mValue || space && !isMoving) && fallDistance < 3.f)
            {
                if (!mAllowMovement.mValue)
                {
                    stateVec ->mVelocity.x = 0;
                    stateVec ->mVelocity.z = 0;
                } else if (!player->isOnGround())
                {
                    glm::vec2 currentMotion = {stateVec ->mVelocity.x, stateVec ->mVelocity.z};
                    float movementSpeed = sqrt(currentMotion.x * currentMotion.x + currentMotion.y * currentMotion.y);
                    float movementYaw = atan2(currentMotion.y, currentMotion.x);
                    float moveYawDeg = movementYaw * (180 / IM_PI) - 90.f;
                    float playerYawDeg = actorRot ->mYaw + MathUtils::getRotationKeyOffset();
                    float yawDiff = playerYawDeg - moveYawDeg;
                    float yawDiffRad = yawDiff * (IM_PI / 180);
                    float newMoveYaw = movementYaw + yawDiffRad;
                    stateVec ->mVelocity.x = cos(newMoveYaw) * movementSpeed;;
                    stateVec ->mVelocity.z = sin(newMoveYaw) * movementSpeed;
                }
                mStartY = player->getPos()->y;
                mIsTowering = true;
                maxExtend = 0.f;
                mShouldClip = true;
            }
            else if (wasTowering)
            {
                mIsTowering = false;
                stateVec ->mVelocity.y = -5.0f;
            }
    }
    case TowerMode::Timer: {
        if (ClientInstance::get()->getMouseGrabbed()) break;
        if ((space && mAllowMovement.mValue || space && !isMoving) && fallDistance < 3.f)
        {
            if (!mAllowMovement.mValue)
            {
                stateVec->mVelocity.x = 0;
                stateVec->mVelocity.z = 0;
            }
            else if (!player->isOnGround())
            {
                glm::vec2 currentMotion = { stateVec->mVelocity.x, stateVec->mVelocity.z };
                float movementSpeed = sqrt(currentMotion.x * currentMotion.x + currentMotion.y * currentMotion.y);
                float movementYaw = atan2(currentMotion.y, currentMotion.x);
                float moveYawDeg = movementYaw * (180 / IM_PI) - 90.f;
                float playerYawDeg = actorRot->mYaw + MathUtils::getRotationKeyOffset();
                float yawDiff = playerYawDeg - moveYawDeg;
                float yawDiffRad = yawDiff * (IM_PI / 180);
                float newMoveYaw = movementYaw + yawDiffRad;
                stateVec->mVelocity.x = cos(newMoveYaw) * movementSpeed;;
                stateVec->mVelocity.z = sin(newMoveYaw) * movementSpeed;
            }
            mStartY = player->getPos()->y;
            mIsTowering = true;
            ClientInstance::get()->getMinecraftSim()->setSimTimer(mTimerSpeed.mValue);
            maxExtend = 0.f;
        }
        else if (wasTowering)
        {
            mIsTowering = false;
            stateVec->mVelocity.y = -5.0f;
            ClientInstance::get()->getMinecraftSim()->setSimTimer(20.0f);
        }
        break;
    }
    }


    if (!BlockUtils::isAirBlock(blockPos) && !mIsTowering)
    {
        for (float i = 0.f; i < maxExtend; i += 1.f)
        {
            blockPos = getPlacePos(i);
            if (BlockUtils::isAirBlock(blockPos)) break;
        }
    }

    if (!BlockUtils::isValidPlacePos(blockPos)) return false;
    if (!BlockUtils::isAirBlock(blockPos)) return false;
    int side = BlockUtils::getBlockPlaceFace(blockPos);

    if (mAvoidUnderplace.mValue && side == 0) return false;

    mLastSwitchTime = NOW;

    if (mLastSlot == -1) mLastSlot = player->getSupplies()->mSelectedSlot;
    int lastSlot = player->getSupplies()->mSelectedSlot;

    if (mSwitchMode.mValue != SwitchMode::None)
    {
        int slot = ItemUtils::getPlaceableItemOnBlock(blockPos, mHotbarOnly.mValue, mSwitchPriority.mValue == SwitchPriority::Highest);
        if (slot == -1) return false;
        if (mSwitchMode.mValue != SwitchMode::Spoof) player->getSupplies()->mSelectedSlot = slot;
        else
        {
            player->getSupplies()->mSelectedSlot = slot;
            PacketUtils::spoofSlot(slot);
        }
    }
    mLastBlock = blockPos;
    mLastFace = side;
    mShouldRotate = true;

    if (mSwing.mValue) player->swing();

    if (mShouldClip)
        player->setPosition(*player->getPos() + glm::vec3(0, 1.f * (mTowerSpeed.mValue / 10), 0));

    BlockUtils::placeBlock(blockPos, side);
    if (mSwitchMode.mValue == SwitchMode::Spoof) {
        player->getSupplies()->mSelectedSlot = lastSlot;
        PacketUtils::spoofSlot(lastSlot);
    }

    return true;
}

void Scaffold::onRenderEvent(RenderEvent& event)
{

    if (mBlockHUDStyle.mValue == BlockHUDStyle::None) return;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    float delta = ImGui::GetIO().DeltaTime;

    static EasingUtil inEase = EasingUtil();
    static float anim = 0.f;
    constexpr float easeSpeed = 10.f;
    this->mEnabled ? inEase.incrementPercentage(delta * easeSpeed / 10)
    : inEase.decrementPercentage(delta * 2 * easeSpeed / 10);
    float inScale = inEase.easeOutExpo();
    if (inEase.isPercentageMax()) inScale = 0.996;
    inScale = MathUtils::clamp(inScale, 0.0f, 0.996);
    anim = MathUtils::lerp(0, 1, inEase.easeOutExpo());

    anim = MathUtils::lerp(anim, mEnabled ? 1.f : 0.f, delta * 10.f);

    if (anim < 0.0001f) return;

    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y * 0.75f - 40);

    int totalBlocks = ItemUtils::getAllPlaceables(mHotbarOnly.mValue);

    std::string displayText = "Blocks: ";
    Interface* daInterface = gFeatureManager->mModuleManager->getModule<Interface>();
    if (daInterface->mNamingStyle.mValue == NamingStyle::Lowercase || daInterface->mNamingStyle.mValue == NamingStyle::LowercaseSpaced)
    {
        displayText = "blocks: ";
    }
    std::string text = displayText + std::to_string(totalBlocks);
    std::string numberText = std::to_string(totalBlocks);

    auto fontSel = daInterface->mFont.as<Interface::FontType>();
    if (fontSel == Interface::FontType::ProductSans) {
        FontHelper::pushPrefFont(true, true);
    }
    else {
        FontHelper::pushPrefFont(true);
    }

    float fontSize = 25.f * anim;

    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, text.c_str());
    // Compensate for anim
    pos.x -= textSize.x / 2;
    pos.y -= textSize.y / 2;

    auto drawList = ImGui::GetBackgroundDrawList();

    drawList->AddShadowRect(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, 0.45f * anim), 500, ImVec2(0, 0));
    drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, (0.45f * anim)), 5.f, 0);

    ImColor color = ImColor(255, 255, 255, 255);

    // Center the text on the bg

    for (int i = 0; i < displayText.size(); i++) {
        color = ColorUtils::getThemedColor(i * 100);
        color.Value.w = color.Value.w * anim;
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, std::string(1, displayText[i]).c_str());

        //Render::RenderText(std::string(1, display[i]), pos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * inScale), true);
        ImVec2 shadowPos = ImVec2(pos.x + 2, pos.y + 2);
        drawList->AddText(ImGui::GetFont(), fontSize, shadowPos, ImColor(color.Value.x * 0.25f, color.Value.y * 0.25f, color.Value.z * 0.25f, 0.9f), std::string(1, displayText[i]).c_str());
        drawList->AddText(ImGui::GetFont(), fontSize, pos, color, std::string(1, displayText[i]).c_str());
        pos.x += ptextSize.x;
    }

    int colorStartingIndex = displayText.size() * 100;

    // render da number
    // Smoothly animate the number
    static std::vector<std::string> numbers = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    static std::string joinedNumbers = StringUtils::join(numbers, "\n");

    int num = totalBlocks;
    std::string numStr = std::to_string(num);

    // Add a cliprect to the drawlist
    ImVec2 numTextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, numStr.c_str());

    ImVec2 clipRectMin = ImVec2(pos.x, pos.y);
    ImVec2 clipRectMax = ImVec2(pos.x + numTextSize.x + 5, pos.y + numTextSize.y);

    drawList->PushClipRect(clipRectMin, clipRectMax, true);

    // Render the number
    for (int i = 0; i < numStr.size(); i++) {
        // Calc text size
        std::string num = std::string(1, numStr[i]);
        int realNum = std::stoi(num);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, num.c_str());

        float offset = ptextSize.y * (realNum);
        static std::map<int, float> indexOffsetMap;

        if (!indexOffsetMap.contains(i)) {
            indexOffsetMap[i] = offset;
        }

        // Scroll the number smoothly to the offset
        ImVec2 daPos = ImVec2((float)i * (float)ptextSize.x + pos.x, (float)-indexOffsetMap[i] + pos.y);

        color = ColorUtils::getThemedColor(colorStartingIndex + i * 100);
        color.Value.w = color.Value.w * anim;

        // Draw the number
        //Render::RenderText(joinedNumbers, daPos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
        ImVec2 shadowPos = ImVec2(daPos.x + 2, daPos.y + 2);
        drawList->AddText(ImGui::GetFont(), fontSize, shadowPos, ImColor(color.Value.x * 0.25f, color.Value.y * 0.25f, color.Value.z * 0.25f, 0.9f * anim), joinedNumbers.c_str());
        drawList->AddText(ImGui::GetFont(), fontSize, daPos, color, joinedNumbers.c_str());
        // we will cliprect this later

        // lerp da offset
        indexOffsetMap[i] = MathUtils::lerp(indexOffsetMap[i], offset, ImGui::GetIO().DeltaTime * 10.f);
    }

    drawList->PopClipRect();


    ImGui::PopFont();

}



void Scaffold::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        if (const auto it = event.getPacket<InventoryTransactionPacket>(); it->mTransaction->type ==
            ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                if (mClickPosition.mValue == ClickPosition::Normal) {
                    transac->mClickPos = BlockUtils::clickPosOffsets[transac->mFace];
                    for (int i = 0; i < 3; i++)
                    {
                        if (transac->mClickPos[i] == 0.5)
                        {
                            transac->mClickPos[i] = MathUtils::randomFloat(-0.49f, 0.49f);
                        }
                    }
                }
                else {
                    transac->mClickPos = { MathUtils::randomFloat(-0.49f, 0.49f), 1.0f, MathUtils::randomFloat(-0.49f, 0.49f) };
                }
                if (mFirstEvent.mValue) {
                    transac->mTriggerType = ItemUseInventoryTransaction::TriggerType::PlayerInput;
                }
            }
        }
    }

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        if (mTest.mValue && !mIsTowering)
        {
            paip->mPos.y = floorf(mStartY) + (1.f + PLAYER_HEIGHT - 0.01f);
        }

        if (mShouldRotate && mRotateMode.mValue != RotateMode::None)
        {
            glm::vec3 side = BlockUtils::blockFaceOffsets[mLastFace] * 0.5f;
            glm::vec3 target = mLastBlock + side;

            auto pos = *player->getPos();

            glm::vec2 rotations = MathUtils::getRots(pos, target);


            if (mRotateMode.mValue == RotateMode::Normal) {
                rotations.y = player->getActorRotationComponent()->mYaw + MathUtils::getRotationKeyOffset();
                if (rotations.y > 180.f) rotations.y -= 360.f;
                if (rotations.y < -180.f) rotations.y += 360.f;
                rotations.x = fmax(82, rotations.x);
            }

            if (mRotateMode.mValue == RotateMode::Down) rotations.x = 89.9f;
            if (mRotateMode.mValue == RotateMode::Backwards)
            {
                rotations.y += 180.f;
                if (rotations.y > 180.f) rotations.y -= 360.f;
                if (rotations.y < -180.f) rotations.y += 360.f;
            }

            bool flickRotate = false;

            auto auraMod = gFeatureManager->mModuleManager->getModule<Aura>();

            if (auraMod->sHasTarget && mFlickMode.mValue == FlickMode::Combat || mFlickMode.mValue == FlickMode::Always) flickRotate = true;

            if (flickRotate) mShouldRotate = false;
            else
            {
                // If the last block placed was more than 500ms ago, then ShouldRotate = false
                if (NOW - mLastSwitchTime > 500) mShouldRotate = false;
            }

            if (auraMod->sHasTarget && mFlickMode.mValue == FlickMode::None)
            {
                mShouldRotate = false;
                flickRotate = false;
                return;
            }

            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
        }
    }
}

glm::vec3 Scaffold::getRotBasedPos(float extend, float yPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    glm::vec2 playerRots = glm::vec2(player->getActorRotationComponent()->mPitch,
                                 player->getActorRotationComponent()->mYaw + MathUtils::getRotationKeyOffset());

    if (mPlacementMode.mValue == PlacementMode::Flareon)
    {
        playerRots.y = MathUtils::snapYaw(playerRots.y);
    }

    float correctedYaw = (playerRots.y + 90) * ((float)IM_PI / 180);
    float inFrontX = cos(correctedYaw) * extend;
    float inFrontZ = sin(correctedYaw) * extend;
    float placeX = player->getPos()->x + inFrontX;
    float placeY = yPos;
    float placeZ = player->getPos()->z + inFrontZ;

    // Floor the values
    return {floor(placeX), floor(placeY), floor(placeZ)};
}

glm::vec3 Scaffold::getPlacePos(float extend)
{
    float yPos = mStartY;
    glm::ivec3 blockSel = {INT_MAX, INT_MAX, INT_MAX};

    blockSel = getRotBasedPos(extend, yPos);

    int side = BlockUtils::getBlockPlaceFace(blockSel);

    if (side == -1)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return {FLT_MAX, FLT_MAX, FLT_MAX};

        if (player->getFallDistance() > 3.f)
        {
            blockSel.y = player->getPos()->y - 3.62f;
        }

        // Find da block
        blockSel = BlockUtils::getClosestPlacePos(blockSel, mRange.as<float>());
        if (blockSel.x == INT_MAX) return {FLT_MAX, FLT_MAX, FLT_MAX};
        side = BlockUtils::getBlockPlaceFace(blockSel);

        if (side == -1) return {FLT_MAX, FLT_MAX, FLT_MAX};
    }

    if (blockSel.x == INT_MAX) return {FLT_MAX, FLT_MAX, FLT_MAX};

    return blockSel;
}