//
// Created by Flash on 9/15/2023.
//

#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class TargetHUD : public ModuleBase<TargetHUD> {
public:
    enum class Style {
        Prax,
        //Emily,
        New,
#ifndef __RELEASEFREE__
        NaCl
#endif
    };

    ModeSetting Style = ModeSetting("Style", "The style of the HUD", {"Prax", /*"Emily",*/ "New",
#ifndef __RELEASEFREE__
        "NaCl"
#endif
    }, 0); // I kinda wanna add a 3d style
    NumberSetting XOffset = NumberSetting("X Offset", "The X offset of the HUD", -400, 400, 100, 1);
    NumberSetting YOffset = NumberSetting("Y Offset", "The Y offset of the HUD", -400, 400, 100, 1);
    BoolSetting AnimateHurtTime = BoolSetting("Animate HurtTime", "Animates the hurt time", true);
    BoolSetting Particles = BoolSetting("Particles", "Shows particles when the player is hurt", true);
    NumberSetting ParticleCount = NumberSetting("Particle Count", "The amount of particles to show", 1, 100, 10, 1, &Particles);
    NumberSetting ParticleRed = NumberSetting("Particle Red", "The red value of the particles", 0, 255, 255, 1, &Particles);
    NumberSetting ParticleGreen = NumberSetting("Particle Green", "The green value of the particles", 0, 255, 0, 1, &Particles);
    NumberSetting ParticleBlue = NumberSetting("Particle Blue", "The blue value of the particles", 0, 255, 0, 1, &Particles);
    NumberSetting ParticleAlpha = NumberSetting("Particle Alpha", "The alpha value of the particles", 0, 255, 255, 1, &Particles);
    BoolSetting HealthDependentColor = BoolSetting("Health Dependent Color", "Whether or not to use health dependent colors", false, &Particles);
    BoolSetting ParticleRGB = BoolSetting("Particle RGB", "Whether or not to use RGB for the particles", false, &Particles);
    NumberSetting Velocity = NumberSetting("Velocity Multiplier", "The velocity of the particles", 0, 10, 1, 0.1, &Particles);
    NumberSetting MaxParticleTime = NumberSetting("Max Particle Time", "The max time the particles will be shown", 0, 3, 1, 1, &Particles);
    BoolSetting AlwaysShowParticles = BoolSetting("Always Show Particles", "Always show particles", false, &Particles);

    TargetHUD() : ModuleBase("TargetHUD", "Shows info about the player you are targeting", "Visual") {
        AddSetting(&Style);
        Style.Display = true;
        AddSetting(&XOffset);
        AddSetting(&YOffset);
        AddSetting(&AnimateHurtTime);
        AddSetting(&Particles);
        AddSetting(&ParticleCount);
        AddSetting(&ParticleRed);
        AddSetting(&ParticleGreen);
        AddSetting(&ParticleBlue);
        AddSetting(&ParticleAlpha);
        AddSetting(&HealthDependentColor);
        AddSetting(&ParticleRGB);
        AddSetting(&Velocity);
        AddSetting(&MaxParticleTime);
        AddSetting(&AlwaysShowParticles);

        RenderOnDisabled = true;
    }

    static std::string PlayerName;
    static int HurtTime;
    static int LastHurtTime;
    static Vector3 LastRenderPos;
    static float LastHealth;
    static float LastAbsorption;
    static float LastMaxHealth;

    void OnTick() override;
    void OnRenderParticles();
    void OnRender() override;
    void OnInitialize() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;

    void RenderNewStyle();
    void RenderEmilyStyle();
    void RenderPraxStyle();
    void RenderNaClStyle();

};
