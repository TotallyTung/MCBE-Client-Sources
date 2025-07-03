#include "StorageESP.h"

int Linealpha = 256;
int renderMode = 2; // 0 = Tracer, 1 = Box, 2 = Both

StorageESP::StorageESP() : Module("ChestVisuals", "Renders storages in render distance", Category::RENDER) {
    registerSetting(new SliderSetting<int>("Radius", "Scanning radius for blocks", &radius, radius, 24, 512));
    registerSetting(new SliderSetting<int>("Alpha", "Opacity of render", &alpha, 128, 0, 255));
    registerSetting(new SliderSetting<int>("Line Alpha", "Line opacity of render", &Linealpha, Linealpha, 0, 255));
    registerSetting(new BoolSetting("Chests", "Render normal chests", &showChests, showChests));
    registerSetting(new BoolSetting("Ender Chests", "Render ender chests", &showEnderChests, showEnderChests));
    registerSetting(new BoolSetting("Shulkers", "Render shulker boxes", &showShulkers, showShulkers));
    registerSetting(new BoolSetting("Barrels", "Render barrels", &showBarrels, showBarrels));
    registerSetting(new BoolSetting("Hoppers", "Render hoppers", &showHoppers, showHoppers));
    registerSetting(new BoolSetting("Furnaces", "Render furnaces", &showFurnaces, showFurnaces));
    registerSetting(new BoolSetting("Spawners", "Render spawners", &showSpawners, showSpawners));
    registerSetting(new BoolSetting("Bookshelves", "Render bookshelves", &showBookshelves, showBookshelves));
    registerSetting(new BoolSetting("Enchant Table", "Render enchanting tables", &showEnchantingTables, showEnchantingTables));
    registerSetting(new BoolSetting("Decorated Pots", "Render decorated pots", &showDecoratedPots, showDecoratedPots));
    registerSetting(new BoolSetting("Cobwebs", "Render cobwebs", &showCobwebs, showCobwebs));
    registerSetting(new BoolSetting("Droppers", "Render droppers", &showDroppers, showDroppers));
    registerSetting(new BoolSetting("Dispensers", "Render dispensers", &showDispensers, showDispensers));
    registerSetting(new BoolSetting("Anvils", "Render anvils", &showAnvils, showAnvils));
    registerSetting(new EnumSetting("Render Mode", "Choose how the storage blocks are rendered", { "Tracer", "Box", "Both" }, &renderMode, 2));
}

void StorageESP::onLevelRender() {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) return;

    Vec3<float> feetPos = localPlayer->getaabbShape()->aabb.getCenter();
    feetPos.y = localPlayer->getaabbShape()->aabb.lower.y;
    BlockPos currentPos = feetPos.add(Vec3<float>(0.f, 0.5f, 0.f)).floor().CastTo<int>();

    // Geçersiz bloklarý temizle
    for (auto it = blockList.begin(); it != blockList.end(); ) {
        BlockPos pos = it->first;

        // Mesafe kontrolü
        if (abs(currentPos.x - pos.x) > radius || abs(currentPos.z - pos.z) > radius) {
            it = blockList.erase(it);
            continue;
        }

        Block* block = Game.clientInstance->getRegion()->getBlock(pos);
        if (!block || !block->blockLegacy || block->blockLegacy->blockId == 0) {
            it = blockList.erase(it);
            continue;
        }

        int blockId = block->blockLegacy->blockId;
        std::string name = block->blockLegacy->blockName;

        bool valid =
            (showChests && (blockId == 54 || blockId == 146)) ||
            (showEnderChests && blockId == 130) ||
            (showShulkers && name.find("shulker") != std::string::npos) ||
            (showBarrels && blockId == 458) ||
            (showHoppers && blockId == 154) ||
            (showFurnaces && (blockId == 61 || blockId == 451 || blockId == 453)) ||
            (showSpawners && blockId == 52) ||
            (showBookshelves && blockId == 47) ||
            (showEnchantingTables && blockId == 116) ||
            (showDecoratedPots && name.find("decorated_pot") != std::string::npos) ||
            (showCobwebs && name.find("web") != std::string::npos) ||
            (showDroppers && name.find("dropper") != std::string::npos) ||
            (showDispensers && blockId == 23) ||
            (showAnvils && blockId == 145);

        if (!valid) {
            it = blockList.erase(it);
        }
        else {
            ++it;
        }
    }

    if (!blockList.empty()) {
        for (const auto& pos : blockList) {
            BlockPos blockPos = pos.first;
            Block* block = Game.clientInstance->getRegion()->getBlock(blockPos);
            if (!block) continue;

            AABB blockAABB;
            block->blockLegacy->getOutline(block, Game.clientInstance->getRegion(), &blockPos, &blockAABB);

            Vec3<float> origin = Game.clientInstance->getLevelRenderer()->levelRendererPlayer->realOrigin;
            float calcYaw = (localPlayer->rotation->presentRot.y + 90.f) * (PI / 180.f);
            float calcPitch = (localPlayer->rotation->presentRot.x) * -(PI / 180.f);

            Vec3<float> moveVec{
                cos(calcYaw) * cos(calcPitch) * 0.2f,
                sin(calcPitch) * 0.2f,
                sin(calcYaw) * cos(calcPitch) * 0.2f
            };

            Vec3<float> start = moveVec;
            Vec3<float> end = blockPos.toFloat().add2(0.5, 0.5, 0.5).sub(origin);
            UIColor boxColor = pos.second;

            if (renderMode == 0 || renderMode == 2) { // Tracer
                MCR::setColor(UIColor(boxColor.r, boxColor.g, boxColor.b, Linealpha));
                MCR::tessellator->begin(VertextFormat::LINE_LIST, 2);
                MCR::tessellator->vertex(start.x, start.y, start.z);
                MCR::tessellator->vertex(end.x, end.y, end.z);
                MeshHelpers::renderMeshImmediately(MCR::screenCtx, MCR::tessellator, MCR::blendMaterial);
            }

            if (renderMode == 1 || renderMode == 2) { // Box
                MCR::drawBox3dFilled(blockAABB, UIColor(boxColor.r, boxColor.g, boxColor.b, alpha), UIColor(boxColor.r, boxColor.g, boxColor.b, Linealpha), 1.f);
            }
        }
    }
}
