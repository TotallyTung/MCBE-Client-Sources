#include "PortalESP.h"
PortalESP::PortalESP() : Module("PortalESP", "Renders Portals in render distance", Category::RENDER) {
    registerSetting(new SliderSetting<int>("Radius", "The more, the slower the module scans for blocks, this is only for horizontal radius, it shouldnt affect fps", &radius, radius, 24, 512));
    registerSetting(new SliderSetting<int>("Alpha", "Opacity of render", &alpha, 128, 0, 255));
    registerSetting(new SliderSetting<int>("Line Alpha", "Line opacity of render", &Linealpha, Linealpha, 0, 255));
}

void PortalESP::onLevelRender() {
    Tessellator* tessellator = MCR::tessellator;

    LocalPlayer* localPlayer = Game.getLocalPlayer();
    AABB playerAABB = localPlayer->getaabbShape()->aabb;

    Vec3<float> feetPos = playerAABB.getCenter();
    feetPos.y = playerAABB.lower.y;

    const BlockPos& currentPos = feetPos.add(Vec3<float>(0.f, 0.5f, 0.f)).floor().CastTo<int>();

    for (auto it = blockList.begin(); it != blockList.end(); ) {
        if (it->second == UIColor(0, 0, 0, 0)) {
            it = blockList.erase(it);
            continue;
        }
        if (abs(currentPos.x - it->first.x) > radius || abs(currentPos.z - it->first.z) > radius || abs(currentPos.y - it->first.y) > radius) {
            it = blockList.erase(it);
            continue;
        }
        it++;
    }

    if (!blockList.empty()) {
        for (const auto& pos : blockList) {
            BlockPos blockPos = pos.first;

            Block* block = Game.clientInstance->getRegion()->getBlock(blockPos);
            AABB blockAABB;
            block->blockLegacy->getOutline(block, Game.clientInstance->getRegion(), &blockPos, &blockAABB);

            UIColor boxColor = pos.second;
            MCR::drawBox3dFilled(blockAABB, UIColor(boxColor.r, boxColor.g, boxColor.b, alpha), UIColor(boxColor.r, boxColor.g, boxColor.g, Linealpha), 1.f);
        }
    }
}