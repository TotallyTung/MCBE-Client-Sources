#pragma once

class Breaker : public Module
{
public:
    Breaker(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Breaker", "Misc", "Fucks objects around you", keybind, enabled)
    {
        registerFloatSetting("Radius", "How far around you is checked for objects", &radius, 1, 12);
        registerBoolSetting("Beds", "Allow fucking of beds", &beds);
        registerBoolSetting("Eggs", "Allow fucking of Eggs", &eggs);
        registerBoolSetting("Cakes", "Allow fucking of Cakes", &cakes);
    }

    float radius = 5;
    bool beds = false,
        eggs = false,
        cakes = true;

    void onEvent(UpdateEvent* event) override
    {
        GameMode* gamemode = Game::GetGameMode();
        Player* player = Game::GetLocalPlayer();

        if (!gamemode || !player)
            return;

        Vector3<int> playerBlockPos = player->GetBlockPosition();

        for (int x = -radius; x <= radius; ++x)
        {
            for (int y = -radius; y <= radius; ++y)
            {
                for (int z = -radius; z <= radius; ++z)
                {
                    Vector3<int> blockPos = playerBlockPos + Vector3<int>(x, y, z);

                    BlockSource* source = Game::GetInstance()->getBlockSource();

                    if (!source)
                        return;

                    Block* block = source->getBlock(blockPos);

                    if (beds && block->GetMaterialType() == MaterialType::Bed)
                        BreakBlock(gamemode, blockPos);

                    if (eggs && block->GetMaterialType() == MaterialType::Egg)
                        BreakBlock(gamemode, blockPos);

                    if (cakes && block->GetMaterialType() == MaterialType::Cake)
                        EatBlock(gamemode, blockPos);
                }
            }
        }
    }

    void BreakBlock(GameMode* gamemode, Vector3<int> blockPos)
    {
        gamemode->destroyBlock(blockPos, Placement::Down, true);
        Game::GetLocalPlayer()->swing();
    }

    void EatBlock(GameMode* gamemode, Vector3<int> blockPos)
    {
        gamemode->buildBlock(blockPos, Placement::Down, false);
        Game::GetLocalPlayer()->swing();
    }

    std::string getModeName() override
    {
        std::ostringstream oss;
        oss.precision(1);
        oss << " " << std::fixed << radius;
        return oss.str();
    }
};