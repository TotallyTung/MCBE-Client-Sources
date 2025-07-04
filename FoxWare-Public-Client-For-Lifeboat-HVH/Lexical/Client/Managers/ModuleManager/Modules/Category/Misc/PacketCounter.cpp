#include "PacketCounter.h"
int xoffset;
int yoffsetXFR;
#include "../../../ModuleManager.h"
float alphax;
float sizexhm;
bool blurbga;
PacketCounter::PacketCounter() : Module("PacketCounter", "Every packet sent is counted", Category::MISC) {
    registerSetting(new BoolSetting("Blur BG", "", &blurbga, false));
    registerSetting(new SliderSetting<float>("Alpha", "", &alphax, 0.f, 0.f, 255.f));
    registerSetting(new SliderSetting<float>("Size", "", &sizexhm, 1.f, 0.1f, 2.f));

    registerSetting(new SliderSetting<int>("Y OFFSET", "", &yoffsetXFR, 0.f, 0.f, 1280.f));
    registerSetting(new SliderSetting<int>("X OFFSET", "", &xoffset, 0.f, 0.f, 1280.f));
}

void PacketCounter::onSendPacket(Packet* packet) {
    static Killaura* kamod = ModuleManager::getModule<Killaura>();
    if (Game::getLocalPlayer()->isDestroying) {
        pargets1++;
    }
    if (packet->getId() == PacketID::KeepAlive) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Login) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayStatus) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ServerToClientHandshake) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientToServerHandshake) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Disconnect) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePacksInfo) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePackStack) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePackClientResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Text) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetTime) {
        pargets1++;
    }
    if (packet->getId() == PacketID::StartGame) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddPlayer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddActor) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RemoveActor) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddItemActor) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UNUSED) {
        pargets1++;
    }
    if (packet->getId() == PacketID::TakeItemActor) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MoveActorAbsolute) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MovePlayer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PassengerJump) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateBlock) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddPainting) {
        pargets1++;
    }
    if (packet->getId() == PacketID::TickSync_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelSoundEventV1) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BlockEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ActorEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MobEffect) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateAttributes) {
        pargets1++;
    }
    if (packet->getId() == PacketID::InventoryTransaction) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MobEquipment) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MobArmorEquipment) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Interact) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BlockPickRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ActorPickRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerAction) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ActorFall_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::HurtArmor) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetActorData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetActorMotion) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetActorLink) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetHealth) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetSpawnPosition) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Animate) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Respawn) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ContainerOpen) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ContainerClose) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerHotbar) {
        pargets1++;
    }
    if (packet->getId() == PacketID::InventoryContent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::InventorySlot) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ContainerSetData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CraftingData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CraftingEvent_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::GuiDataPickItem) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AdventureSettings_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BlockActorData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerInput) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelChunk) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetCommandsEnabled) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetDifficulty) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ChangeDimension) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetPlayerGameType) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerList) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SimpleEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LegacyTelemetryEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SpawnExperienceOrb) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientboundMapItemData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MapInfoRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RequestChunkRadius) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ChunkRadiusUpdated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ItemFrameDropItem_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::GameRulesChanged) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Camera) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BossEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ShowCredits) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AvailableCommands) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CommandRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CommandBlockUpdate) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CommandOutput) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateTrade) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateEquip) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePackDataInfo) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePackChunkData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ResourcePackChunkRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Transfer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlaySound) {
        pargets1++;
    }
    if (packet->getId() == PacketID::StopSound) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetTitle) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddBehaviorTree) {
        pargets1++;
    }
    if (packet->getId() == PacketID::StructureBlockUpdate) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ShowStoreOffer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PurchaseReceipt) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerSkin) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SubClientLogin) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AutomationClientConnect) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetLastHurtBy) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BookEdit) {
        pargets1++;
    }
    if (packet->getId() == PacketID::NpcRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PhotoTransfer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ModalFormRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ModalFormResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ServerSettingsRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ServerSettingsResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ShowProfile) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetDefaultGameType) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RemoveObjective) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetDisplayObjective) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetScore) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LabTable) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateBlockSynced) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MoveActorDelta) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetScoreboardIdentity) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetLocalPlayerAsInit) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateSoftEnum) {
        pargets1++;
    }
    if (packet->getId() == PacketID::NetworkStackLatency) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BlockPalette) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ScriptCustomEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SpawnParticleEffect) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AvailableActorIdentifiers) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelSoundEventV2) {
        pargets1++;
    }
    if (packet->getId() == PacketID::NetworkChunkPublisherUpdate) {
        pargets1++;
    }
    if (packet->getId() == PacketID::BiomeDefinitionList) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelSoundEvent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LevelEventGeneric) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LecternUpdate) {
        pargets1++;
    }
    if (packet->getId() == PacketID::VideoStreamConnect_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddEntity_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RemoveEntity_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientCacheStatus) {
        pargets1++;
    }
    if (packet->getId() == PacketID::OnScreenTextureAnimation) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MapCreateLockedCopy) {
        pargets1++;
    }
    if (packet->getId() == PacketID::StructureTemplateDataRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::StructureTemplateDataResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UNUSED_PLS_USE_ME2) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientCacheBlobStatus) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientCacheMissResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::EducationSettings) {
        pargets1++;
    }
    if (packet->getId() == PacketID::Emote) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MultiplayerSettings) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SettingsCommand) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AnvilDamage) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CompletedUsingItem) {
        pargets1++;
    }
    if (packet->getId() == PacketID::NetworkSettings) {
        pargets1++;
    }
    if (packet->getName() == "PlayerAuthInputPacket") {
        pargets1++;
    }
    if (packet->getId() == PacketID::CreativeContent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerEnchantOptions) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ItemStackRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ItemStackResponse) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerArmorDamage) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CodeBuilder) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdatePlayerGameType) {
        pargets1++;
    }
    if (packet->getId() == PacketID::EmoteList) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PositionTrackingDBServerBroadcast) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PositionTrackingDBClientRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::DebugInfo) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PacketViolationWarning) {
        pargets1++;
    }
    if (packet->getId() == PacketID::MotionPredictionHints) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AnimateEntity) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CameraShake) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerFog) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CorrectPlayerMovePrediction) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ItemComponent) {
        pargets1++;
    }
    if (packet->getId() == PacketID::FilterText_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientboundDebugRenderer) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SyncActorProperty) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AddVolumeEntity) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RemoveVolumeEntity) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SimulationType) {
        pargets1++;
    }
    if (packet->getId() == PacketID::NpcDialogue) {
        pargets1++;
    }
    if (packet->getId() == PacketID::EduUriResource) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CreatePhoto) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateSubChunkBlocks) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PhotoInfoRequest_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SubChunk) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SubChunkRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerStartItemCooldown) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ScriptMessage) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CodeBuilderSource) {
        pargets1++;
    }
    if (packet->getId() == PacketID::TickingAreasLoadStatus) {
        pargets1++;
    }
    if (packet->getId() == PacketID::DimensionData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AgentAction) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ChangeMobProperty) {
        pargets1++;
    }
    if (packet->getId() == PacketID::LessonProgress) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RequestAbility) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RequestPermissions) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ToastRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateAbilities) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateAdventureSettings) {
        pargets1++;
    }
    if (packet->getId() == PacketID::DeathInfo) {
        pargets1++;
    }
    if (packet->getId() == PacketID::EditorNetwork) {
        pargets1++;
    }
    if (packet->getId() == PacketID::FeatureRegistry) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ServerStats) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RequestNetworkSettings) {
        pargets1++;
    }
    if (packet->getId() == PacketID::GameTestRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::GameTestResults) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UpdateClientInputLocks) {
        pargets1++;
    }
    if (packet->getId() == PacketID::ClientCheatAbility_Deprecated) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CameraPresets) {
        pargets1++;
    }
    if (packet->getId() == PacketID::UnlockedRecipes) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CameraInstruction) {
        pargets1++;
    }
    if (packet->getId() == PacketID::CompressedBiomeDefinitionList) {
        pargets1++;
    }
    if (packet->getId() == PacketID::TrimData) {
        pargets1++;
    }
    if (packet->getId() == PacketID::OpenSign) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AgentAnimation) {
        pargets1++;
    }
    if (packet->getId() == PacketID::RefreshEntitlements) {
        pargets1++;
    }
    if (packet->getId() == PacketID::PlayerToggleCrafterSlotRequest) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetPlayerInventoryOptions) {
        pargets1++;
    }
    if (packet->getId() == PacketID::SetHud) {
        pargets1++;
    }
    if (packet->getId() == PacketID::AwardAchievementPacket) {
        pargets1++;
    }
}

void PacketCounter::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr)
        return;

    static Colors* coie = ModuleManager::getModule<Colors>();
    UIColor mainColor = UIColor(coie->getColor());

    Vec2<float> watermarkPos(xoffset, yoffsetXFR);
    std::string watermarkText = "packets sent: " + std::to_string(pargets1);

    float textWidth = D2D::getTextWidth(watermarkText, sizexhm, false);
    float textHeight = D2D::getTextHeight(watermarkText, sizexhm, false);

    Vec4<float> watermarkRect(
        watermarkPos.x,
        watermarkPos.y,
        watermarkPos.x + textWidth,
        watermarkPos.y + textHeight
    );

    D2D::fillRectangle(watermarkRect, UIColor(0, 0, 0, alphax));
    if (blurbga)
        D2D::addBlur(watermarkRect, 10.f);

    D2D::drawText(watermarkPos, watermarkText, UIColor(mainColor), sizexhm, false);
}