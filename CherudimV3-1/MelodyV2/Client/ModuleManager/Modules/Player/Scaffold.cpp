#include "Scaffold.h"

Scaffold::Scaffold() : Module("Scaffold", "Op scaffold by toni.", Category::PLAYER) {
    addSlider<int>("Place ticks", "Ticks per blocks place", ValueType::INT_T, &placeticks, 1, 30);
	addBoolCheck("Rotation", "Silent is enough", &rot);
}
bool hasTowered = false;
bool Scaffold::canPlace(Vec3<float> pos) {
    return mc.getLocalPlayer()->dimension->blockSource->getBlock(pos.floor().toInt())->blockLegacy->blockId == 0;
}

void Scaffold::onEnable() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    hasTowered = false;
}
bool Scaffold::predictBlock(Vec3<float> pos, GameMode* gm) {
    static std::vector<Vec3<float>> blocks;
    if (blocks.empty()) {
        for (int y = -5; y <= 0; y++) {
            for (int x = -5; x <= 5; x++) {
                for (int z = -5; z <= 5; z++) {
                    blocks.push_back(Vec3<float>(x, y, z));
                }
            }
        }
        std::sort(blocks.begin(), blocks.end(), [](Vec3<float> start, Vec3<float> end) {
            return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
            });
    }

    for (const Vec3<float>& offset : blocks) {
        Vec3<float> currentBlock = Vec3<float>(pos.floor()).add(offset);
        if (gm->tryPlaceBlock(currentBlock.toInt())) {
            return true;
        }
    }
    return false;
}

void Scaffold::onNormalTick(Actor* actor) {
    LocalPlayer* player = (LocalPlayer*)actor;
    GameMode* gm = mc.getGameMode();
    if (!player || !gm) {
        return;
    }
    auto velocity = player->stateVectorComponent->velocity;
   // Vec3<float> velocity = stateVec.normalize();

    // Set our yLevel
    if (player->isOnGround() || player->getPosition()->y < targetY + 0.5f || velocity != Vec3<float>(0, 0, 0) && mc.isKeyDown(VK_SPACE) || (!false && player->getPosition()->y > targetY))
        targetY = player->getPosition()->floor().y - 0.5f;

    // Handle tower modes
    if (velocity != Vec3<float>(0, 0, 0) && mc.isKeyDown(VK_SPACE) && velocity.y > -0.2) {
        velocity.y = .85f;
        hasTowered = true;
    }
    else {
        if (!player->isOnGround() && hasTowered) {
            velocity.y = -5.f;
            hasTowered = false;
        }
    }

    targetBlock = mc.getLocalPlayer()->stateVectorComponent->pos;
    targetBlock.y = targetY;
    rotAngle = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(targetBlock);
    targetBlock.x += velocity.x * 3.5f;
    targetBlock.z += velocity.z * 3.5f;
    if (canPlace(targetBlock)) {
        gm->buildBlock(targetBlock.toInt(), -1,false);
    }
}

void Scaffold::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (rot && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}

	if (rot && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->rotation = rotAngle;
		movepacket->headYaw = rotAngle.y;
		movepacket->onGround = true;
		movepacket->tick = 20;
	}
}
