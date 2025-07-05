//
// Created by vastrakai on 4/27/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>


class DamageEffects : public ModuleBase<DamageEffects>
{
public:
    BoolSetting DamageParticles = BoolSetting("Damage Particles", "Drops particles when someone takes damage", true);
    NumberSetting ParticleCount = NumberSetting("Particle Count", "How many particles to drop", 1, 100, 10, 1, &DamageParticles);
    NumberSetting ParticleSize = NumberSetting("Particle Size", "The size of the particles", 0, 20, 5.f, 0.1, &DamageParticles);
    NumberSetting MinLife = NumberSetting("Min Life", "The minimum life of a particle", 0, 10, 0.75f, 0.1, &DamageParticles);
    NumberSetting MaxLife = NumberSetting("Max Life", "The maximum life of a particle", 0, 10, 1.25f, 0.1, &DamageParticles);
    BoolSetting ThemedColors = BoolSetting("Themed Colors", "Use themed colors for the particles", true);

    DamageEffects() : ModuleBase("DamageEffects", "Visual effects for damage", "Visual", 0, false) {
        AddSetting(&DamageParticles);
        AddSetting(&ParticleCount);
        AddSetting(&ParticleSize);
        AddSetting(&MinLife);
        AddSetting(&MaxLife);
        AddSetting(&ThemedColors);
    }

    // inner types

    struct Particle
    {
        Vector3 Position;
        Vector3 Velocity;
        uint64_t StartTime;
        float MaxLife;
        ImColor Color;
        float FloorY;
        std::vector<AABB> BlocksNearby;
    };

    // static fields
    static std::vector<Particle> Particles;

    // methods
    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;

};