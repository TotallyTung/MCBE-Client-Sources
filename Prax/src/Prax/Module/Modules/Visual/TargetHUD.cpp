//
// Created by Flash on 9/15/2023.
//

#include "TargetHUD.h"

#include <iostream>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/String.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Network/Other/ActorEventPacket.h>

#define DRAW_LIST ImGui::GetForegroundDrawList()

std::string TargetHUD::PlayerName = "";
int TargetHUD::HurtTime = 0;
int TargetHUD::LastHurtTime = 0;
float TargetHUD::LastHealth = 0;
float TargetHUD::LastAbsorption = 0;
float TargetHUD::LastMaxHealth = 0;

// TODO
// documentation of insanity: i am gonna reverse a ton of bgfx later, taking a shot at under ui, maybe... - 26/04/2024
struct TargetInfo {
    std::string Name;
    int HurtTime;
    float Health;
    float Absorption;
    float MaxHealth;
};

void TargetHUD::OnTick(){
    if (!Aura::HasTarget) return;
    auto player = Minecraft::ClientInstance->ThePlayer();

    Actor* actor = Aura::LastTargetedActor;
    if (!actor) return;

    if (actor->isPlayer() && actor->hurtTime) {
        LastHurtTime = HurtTime;
        HurtTime = *actor->hurtTime;
    }

    std::string nameTagString = actor->_getNameTag();

    for (auto& pair : player->level->playerList) {
        const std::string& name = pair.second.name;

        if (String::ContainsIgnoreCase(nameTagString, name)){
            PlayerName = name;
            break;
        }
    }

    LastHealth = actor->getHealth();
    LastAbsorption = actor->getAbsorption();
    LastMaxHealth = actor->getMaxHealth();
}

void TargetHUD::OnInitialize() {
}

float AnimationPerc = 0.f;
float HealthAnimationPerc = 0.f;
float AbHealthAnimationPerc = 0.f;
float hurtTimeAnimPerc = 0.f;

Vector3 TargetHUD::LastRenderPos = Vector3();



struct Particle {
    ImVec2 pos;
    ImVec2 vel;
    float time;
    float maxTime;
    ImColor color;
};


void TargetHUD::RenderPraxStyle() {

    if (!Enabled) return;
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    if (!Prax::ShouldUIRender) return;
    if (!Aura::LastTargetedActor &&  0.6f * AnimationPerc == 1.f || !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f || !Aura::LastTargetedActor && !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f) return;



    float delta = ImGui::GetIO().DeltaTime;

    bool auraHasTarget = Aura::HasTarget && Aura::LastTargetedActor && Enabled;

    AnimationPerc = Math::Animate(AnimationPerc, auraHasTarget ? 1.f : 0.f,
                                  delta * 12.f);
    AnimationPerc = std::clamp(AnimationPerc, 0.f, 1.f);

    ImVec2 size = ImVec2(200, 75);

    // World to screen the player
    static Vector2 screenPos = {0, 0};
    Vector2 screenPos2 = {0, 0};


    if (!Aura::LastTargetedActor) {
        // animate the position

    }

    screenPos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value,
                       ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);


    if (screenPos.x == 0 || screenPos.y == 0)
        return;


    float distanceValue = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive())
        distanceValue = Aura::LastTargetedActor->GetAABB().GetClosestPoint((*Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer())))
                .Distance(*Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer()));

    ImVec2 pos = screenPos;

    pos.y = Math::Lerp(pos.y + size.y, pos.y, AnimationPerc);

    float lerpedHurtTime = Math::Animate(LastHurtTime / 10.f, HurtTime / 10.f, delta);
    hurtTimeAnimPerc = Math::Animate(hurtTimeAnimPerc, lerpedHurtTime, delta * 20.f);
    if (!AnimateHurtTime.Enabled) hurtTimeAnimPerc = 0.f;
    ImColor color = ImColor(hurtTimeAnimPerc, 0.f, 0.f, 0.6f * AnimationPerc);

    ImVec2 shadowPos = ImVec2(pos.x + 5, pos.y + 5);
    DRAW_LIST->AddShadowRect(shadowPos, ImVec2(pos.x + size.x, shadowPos.y + size.y), color, 50.0f,
                             ImVec2(1.0f, 1.0f), 0.0f);
    D2D::AddBlur(DRAW_LIST, 10.f * AnimationPerc, ImVec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y));
    DRAW_LIST->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color, 5.f);

    float distanceFromLP = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive())
        distanceFromLP = Misc::GetLerpedActorPos(Aura::LastTargetedActor).Distance(
                *Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer()));

    // limit the distance to 2 decimal places
    std::string distance =
            "Distance: " + std::to_string(distanceFromLP).substr(0, std::to_string(distanceFromLP).find('.') + 3);

    // Render using bold greycliff
    DRAW_LIST->AddText(Font::Fonts["greycliff_bold_large"], 20, ImVec2(pos.x + 10, pos.y + 10),
                       ImColor(255, 255, 255, (int) (255 * AnimationPerc)), PlayerName.c_str(), nullptr, size.x - 20);

    // Render using medium greycliff
    DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"], 15, ImVec2(pos.x + 10, pos.y + 40),
                       ImColor(255, 255, 255, (int) (255 * AnimationPerc)), distance.c_str(), nullptr, size.x - 20);

    // Add the health bar
    float perc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {
        perc = LastHealth / LastMaxHealth;
    }

    HealthAnimationPerc = Math::Animate(HealthAnimationPerc, perc, delta * 6.f);

    // Render the health bar
    DRAW_LIST->AddRectFilled(ImVec2(pos.x, pos.y + size.y - 5),
                             ImVec2(pos.x + size.x * HealthAnimationPerc, pos.y + size.y),
                             ImColor(249, 98, 94, (int) (255 * AnimationPerc)));

    float abPerc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {

        abPerc = LastAbsorption / 20.f;
    }

    AbHealthAnimationPerc = Math::Animate(AbHealthAnimationPerc, abPerc, delta * 6.f);

    // Render the absorption bar
    DRAW_LIST->AddRectFilled(ImVec2(pos.x, pos.y + size.y - 5), ImVec2(pos.x + size.x * AbHealthAnimationPerc, pos.y + size.y),
                             ImColor(255, 229, 61, (int)(255 * AnimationPerc)));

}
void TargetHUD::RenderEmilyStyle() {

    if (!Enabled) return;
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    if (!Prax::ShouldUIRender) return;
    if (!Aura::LastTargetedActor &&  0.6f * AnimationPerc == 1.f || !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f || !Aura::LastTargetedActor && !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f) return;

    float delta = ImGui::GetIO().DeltaTime;

    bool auraHasTarget = Aura::HasTarget && Aura::LastTargetedActor && Enabled;

    AnimationPerc = Math::Animate(AnimationPerc, auraHasTarget ? 1.f : 0.f,
                                  delta * 12.f);
    AnimationPerc = std::clamp(AnimationPerc, 0.f, 1.f);

    // EVERYTHING should depend on this sizeScale and scale properly
    float sizeScale = AnimationPerc;

    ImVec2 size = ImVec2(200 * sizeScale, 75 * sizeScale);

    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value,
                       ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);

    if (pos.x == 0 || pos.y == 0)
        return;

    pos.x -= size.x / 2;
    pos.y -= size.y / 2;



    float distanceValue = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive())
        distanceValue = Aura::LastTargetedActor->GetAABB().GetClosestPoint((*Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer())))
                .Distance(*Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer()));

    // pos.y = Math::Lerp(pos.y + size.y, pos.y, AnimationPerc); // dont use this, we're going to make a zoom in/out effect

    float lerpedHurtTime = Math::Animate(LastHurtTime / 10.f, HurtTime / 10.f, delta);
    hurtTimeAnimPerc = Math::Animate(hurtTimeAnimPerc, lerpedHurtTime, delta * 20.f);
    if (!AnimateHurtTime.Enabled) hurtTimeAnimPerc = 0.f;
    ImColor color = ImColor(hurtTimeAnimPerc, 0.f, 0.f, 0.6f * AnimationPerc);

    ImVec2 shadowPos = ImVec2(pos.x + 5, pos.y + 5);
    DRAW_LIST->AddShadowRect(shadowPos, ImVec2(pos.x + size.x, shadowPos.y + size.y), color, 50.0f,
                             ImVec2(1.0f, 1.0f), 0.0f);
    D2D::AddBlur(DRAW_LIST, 10.f * AnimationPerc, ImVec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y));
    DRAW_LIST->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color, 5.f);

    float distanceFromLP = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive())
        distanceFromLP = Misc::GetLerpedActorPos(Aura::LastTargetedActor).Distance(
                *Misc::GetLerpedActorPos(Minecraft::ClientInstance->ThePlayer()));

    // limit the distance to 2 decimal places
    std::string distance =
            "Distance: " + std::to_string(distanceFromLP).substr(0, std::to_string(distanceFromLP).find('.') + 3);

    // Render using bold greycliff
    DRAW_LIST->AddText(Font::Fonts["greycliff_bold_large"], 20 * sizeScale, ImVec2(pos.x + 10 * sizeScale, pos.y + 10 * sizeScale),
                       ImColor(255, 255, 255, (int) (255 * AnimationPerc)), PlayerName.c_str(), nullptr, size.x - 20 * sizeScale);

    // Render using medium greycliff
    DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"], 15 * sizeScale, ImVec2(pos.x + 10 * sizeScale, pos.y + 40 * sizeScale),
                       ImColor(255, 255, 255, (int) (255 * AnimationPerc)), distance.c_str(), nullptr, size.x - 20 * sizeScale);

    // Add the health bar
    float perc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {
        perc = LastHealth / LastMaxHealth;
    }

    HealthAnimationPerc = Math::Animate(HealthAnimationPerc, perc, delta * 6.f);

    // Render the health bar
    if (HealthAnimationPerc > 0.01f)
        DRAW_LIST->AddRectFilled(ImVec2(pos.x, pos.y + size.y - 5 * sizeScale),
                                 ImVec2(pos.x + size.x * HealthAnimationPerc, pos.y + size.y),
                                 ImColor(249, 98, 94, (int) (255 * AnimationPerc)), 5.f);

    float abPerc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {

        abPerc = LastAbsorption / 20.f;
    }

    AbHealthAnimationPerc = Math::Animate(AbHealthAnimationPerc, abPerc, delta * 6.f);

    if (AbHealthAnimationPerc > 0.01f)
        DRAW_LIST->AddRectFilled(ImVec2(pos.x, pos.y + size.y - 5 * sizeScale), ImVec2(pos.x + size.x * AbHealthAnimationPerc, pos.y + size.y),
                                 ImColor(255, 229, 61, (int)(255 * AnimationPerc)), 5.f);

};

void TargetHUD::RenderNaClStyle() {

    if (!Enabled) return;
    if (!Minecraft::ClientInstance->ThePlayer()) return;

    const bool hasTarget = Aura::HasTarget && Aura::LastTargetedActor;

    static float anim = 0;
    static float absAnim = 0;

    const auto delta = ImGui::GetIO().DeltaTime;

    if (hasTarget) {
        anim = Math::Lerp(anim, 1, delta * 10 * (anim > 0.75 ? 2 * anim : (3 * (1 - anim))));
        absAnim = Math::Lerp(absAnim, LastAbsorption, delta * 10 * (fmodf(absAnim, 1.f) > 0.75 ? 2 * fmodf(absAnim, 1.f) : (3 * (1 - fmodf(absAnim, 1.f)))));
    } else {
        anim = Math::Lerp(anim, 0, delta * 10 * (anim > 0.75 ? 2 * anim : (3 * (1 - anim))));;
        absAnim = Math::Lerp(absAnim, 0, delta * 10 * (fmodf(absAnim, 1.f) > 0.75 ? 2 * fmodf(absAnim, 1.f) : (3 * (1 - fmodf(absAnim, 1.f)))));
    }

    if (!Prax::ShouldUIRender) {
        return;
    }

    if (anim == 0) return;

    const auto size = ImGui::GetIO().DisplaySize;

    static float _width = 0, _height = 0;

    if (_width == 0) {
        _width = 185;
        _height = 34;
    }

    float _x = size.x / 2 + 44, _y = size.y / 2 - _height / 2 - 10;

    static float _minWidth = 0;

    if (_minWidth == 0) {
        _minWidth = _width - 46;
    }

    const auto bgColor = ImColor(0, 0, 0, static_cast<int>(150 * anim));

    const float width = _minWidth + 46 * anim, height = _height, x = _x + (46 * (1 - anim)) / 2, y = _y + 23 * (1 - anim);

    const auto fgList = ImGui::GetForegroundDrawList();
    const auto bgList = ImGui::GetBackgroundDrawList();

    Render::PushDrawList(bgList);

    Render::DrawRect(ImVec2(x, y), ImVec2(x + width, y + height), bgColor);

    const auto accentColor = ImColor(255, 255, 255, static_cast<int>(255 * anim));

    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x + width, y), accentColor, 1.5);

    Render::PushDrawList(fgList);

    Render::FillAreaWithBlur(3 * anim - 0.2, ImVec4(x, y, x + width, y + height));

    Render::DrawGlowLine(ImVec2(x, y), ImVec2(x + width, y), accentColor, 1.5);

    const auto font = Font::Fonts["sarabun_light"];

    ImGui::PushFont(font);

    const float font_size = ceil((19 + 6 * anim) * 10) / 10.f;

    bgList->AddText(font, font_size, ImVec2(x + 10, y + (height / 2) - (font_size / 2)), accentColor, PlayerName.c_str());
    fgList->AddText(font, font_size, ImVec2(x + 10, y + (height / 2) - (font_size / 2)), accentColor, PlayerName.c_str());

    bgList->AddCircle(ImVec2(x + width - 15, y + height / 2), absAnim, accentColor, 24, 1.f);
    fgList->AddCircle(ImVec2(x + width - 15, y + height / 2), absAnim, accentColor, 24, 1.f);

    Render::PopDrawList();

    ImGui::PopFont();

}

void TargetHUD::RenderNewStyle()
{
    if (!Enabled) return;
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    if (!Prax::ShouldUIRender) return;
    if (!Aura::LastTargetedActor &&  0.6f * AnimationPerc == 1.f || !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f || !Aura::LastTargetedActor && !Aura::HasTarget &&  0.6f * AnimationPerc == 1.f)
    {
        OnRenderParticles();
        return;
    }

    float delta = ImGui::GetIO().DeltaTime;

    bool auraHasTarget = Aura::HasTarget && Aura::LastTargetedActor && Enabled;

    AnimationPerc = Math::Animate(AnimationPerc, auraHasTarget ? 1.f : 0.f,
                                  delta * 12.f);
    AnimationPerc = std::clamp(AnimationPerc, 0.f, 1.f);

    // EVERYTHING should depend on this sizeScale and scale properly
    float sizeScale = AnimationPerc;

    ImVec2 size = ImVec2(300 * sizeScale, 85 * sizeScale);

    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value,
                       ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);

    if (pos.x == 0 || pos.y == 0)
    {
        OnRenderParticles();
        return;
    }

    pos.x -= size.x / 2;
    pos.y -= size.y / 2;

    float lerpedHurtTime = Math::Animate(LastHurtTime / 10.f, HurtTime / 10.f, delta);
    hurtTimeAnimPerc = Math::Animate(hurtTimeAnimPerc, lerpedHurtTime, delta * 20.f);
    if (!AnimateHurtTime.Enabled) hurtTimeAnimPerc = 0.f;
    ImColor color = ImColor(hurtTimeAnimPerc, 0.f, 0.f, 0.6f * AnimationPerc);

    // Render the background
    DRAW_LIST->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(0.f, 0.f, 0.f, 0.6f * AnimationPerc), 10.f);



    // Render the name of the player
    float fontSize = 20 * sizeScale;
    ImVec2 textPos = ImVec2(pos.x + 85 * sizeScale, pos.y + 15 * sizeScale);

    DRAW_LIST->AddText(Font::Fonts["greycliff_bold_large"], fontSize, textPos,
                       ImColor(255, 255, 255, (int) (255 * AnimationPerc)), PlayerName.c_str(), nullptr, size.x - 20 * sizeScale);

    ImVec2 healthBarStart = ImVec2(pos.x + 85 * sizeScale, pos.y + 39 * sizeScale);
    ImVec2 healthBarEnd = ImVec2(pos.x + size.x - 30 * sizeScale, pos.y + 68 * sizeScale);

    // Draw the background of the health bar
    DRAW_LIST->AddRectFilled(healthBarStart, healthBarEnd,
                             ImColor(149, 130, 133, (int)((float)194 * AnimationPerc)), 10.f);

    // Render the health bar
    float perc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {
        perc = LastHealth / LastMaxHealth;
    }

    HealthAnimationPerc = Math::Animate(HealthAnimationPerc, perc, delta * 6.f);

    float abPerc = 0;
    if (Aura::LastTargetedActor && Aura::HasTarget && Aura::LastTargetedActor->isAlive()) {

        abPerc = LastAbsorption / 20.f;
    }

    AbHealthAnimationPerc = Math::Animate(AbHealthAnimationPerc, abPerc, delta * 6.f);

    // Render the health bar
    if (HealthAnimationPerc > 0.01f)
        DRAW_LIST->AddRectFilled(healthBarStart, ImVec2(healthBarStart.x + (healthBarEnd.x - healthBarStart.x) * HealthAnimationPerc, healthBarEnd.y),
                                 ImColor(213, 187, 190, (int) (255 * AnimationPerc)), 10.f);



    if (AbHealthAnimationPerc > 0.01f)
        DRAW_LIST->AddRectFilled( healthBarStart,
                                    ImVec2(healthBarStart.x + (healthBarEnd.x - healthBarStart.x) * AbHealthAnimationPerc, healthBarEnd.y),
                                 ImColor(234, 168, 95, (int)(255 * AnimationPerc)), 10.f);

    bool renderAbsorptionText = LastAbsorption > 0;
    if (renderAbsorptionText) {
        std::string absorptionText = "+" + std::to_string((int) LastAbsorption);

        if (sizeScale > 0.99f) sizeScale = 1.f;

        // Render to the right middle of the health bar bg
        ImVec2 hbs = healthBarStart;
        ImVec2 hbe = healthBarEnd;
        ImVec2 absorptionTextPos = ImVec2(hbe.x, hbs.y + (hbe.y - hbs.y) / 2 - 8);

        DRAW_LIST->AddText(Font::Fonts["greycliff_medium_large"], 20 * sizeScale, absorptionTextPos,
                           ImColor(255, 149, 0, (int) (255 * AnimationPerc)), absorptionText.c_str(), nullptr, size.x + 20 * sizeScale);
    }

    {
        ImVec2 headSize = ImVec2(65 * sizeScale, 65 * sizeScale);
        ImVec2 headPos = ImVec2(pos.x + 10 * sizeScale, pos.y + 10 * sizeScale); // this is the center of the headpos

        ImVec2 headSize2 = ImVec2(Math::Lerp(headSize.x, 40 * sizeScale, hurtTimeAnimPerc), Math::Lerp(headSize.y, 40 * sizeScale, hurtTimeAnimPerc));

        // Center the headPos
        headPos.x += (headSize.x - headSize2.x) / 2;
        headPos.y += (headSize.y - headSize2.y) / 2;

        // Render the head of the player
        ImColor imageColor = ImColor(1.f, 1.f, 1.f, 1.f * AnimationPerc);

        // Adjust the color based on hurttime (red tint)
        imageColor.Value.x = Math::Lerp(imageColor.Value.x, 1.f, hurtTimeAnimPerc);
        imageColor.Value.y = Math::Lerp(imageColor.Value.y, 1.f - hurtTimeAnimPerc, hurtTimeAnimPerc);
        imageColor.Value.z = Math::Lerp(imageColor.Value.z, 1.f - hurtTimeAnimPerc, hurtTimeAnimPerc);

        OnRenderParticles();

        Render::DrawRoundedImageFromURL("https://vastrakai.github.io/Website/assets/notch.png", headPos, headSize2, 10.f, imageColor);
    }

}



static std::vector<Particle> particles;

void TargetHUD::OnRenderParticles()
{
    if (!Particles.Enabled) return;

    float delta = ImGui::GetIO().DeltaTime;

    for (int i = 0; i < particles.size(); i++) {
        Particle& particle = particles[i];
        particle.pos.x += particle.vel.x * delta;
        particle.pos.y += particle.vel.y * delta;
        // decrease velocity
        particle.vel.x = Math::Lerp(particle.vel.x, 0, delta * 3);
        particle.vel.y = Math::Lerp(particle.vel.y, 0, delta * 3);
        particle.time += delta;
        if (particle.time > particle.maxTime) {
            particles.erase(particles.begin() + i);
            i--;
        }
    }

    for (Particle& particle : particles) {
        // Add a red shadow behind it
        int r = particle.color.Value.x * 255;
        int g = particle.color.Value.y * 255;
        int b = particle.color.Value.z * 255;
        float ap = AnimationPerc;
        if (AlwaysShowParticles.Enabled) ap = 1.f;

        ImColor particleColor = ImColor(r, g, b, (int)((ParticleAlpha.Value * (1 - particle.time / particle.maxTime) * ap)));
        ImColor rainbowColor = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, 10);

        if (ParticleRGB.Enabled) {
            particleColor = ImColor(rainbowColor.Value.x, rainbowColor.Value.y, rainbowColor.Value.z, particleColor.Value.w);
        }


        DRAW_LIST->AddShadowCircle(ImVec2(particle.pos.x, particle.pos.y), 7, particleColor, 10.f, ImVec2(0, 0), 0.f);
        DRAW_LIST->AddCircleFilled(particle.pos, 5, particleColor);
    }
}

void TargetHUD::OnRender() {
    switch (Style.SelectedIndex) {
        case (int)Style::Prax:
            RenderPraxStyle();
            OnRenderParticles();
            break;
        /*case (int)Style::Emily:
            RenderEmilyStyle();
            OnRenderParticles();
            break;*/
        case (int)Style::New:
            RenderNewStyle();
            break;
#ifndef __RELEASEFREE__
        case static_cast<int>(Style::NaCl):
            RenderNaClStyle();
            break;
#endif
    }
}

void TargetHUD::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) {
    if (packet->getId() == (int)PacketID::EntityEvent) {
        auto eep = std::reinterpret_pointer_cast<ActorEventPacket>(packet);

        if (Aura::HasTarget && Aura::LastTargetedActor && Aura::LastTargetedActor->GetRuntimeID() == eep->ActorRuntimeID && eep->Event == ActorEvent::HURT) {
            // Get the size and pos (based on Emily mode)
            ImVec2 size;
            ImVec2 pos;

            /*if (Style.SelectedIndex == (int)Style::Emily)
            {
                size = ImVec2(200, 75);
                pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value, ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);
                pos.x -= size.x / 2;
                pos.y -= size.y / 2;
            }
            else */if (Style.SelectedIndex == (int)Style::New)
            {
                float sizeScale = AnimationPerc;

                ImVec2 dsize = ImVec2(300 * sizeScale, 85 * sizeScale);

                ImVec2 dpos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value,
                                   ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);

                if (dpos.x == 0 || dpos.y == 0)
                    return;

                dpos.x -= dsize.x / 2;
                dpos.y -= dsize.y / 2;

                ImVec2 headSize = ImVec2(65 * sizeScale, 65 * sizeScale);
                ImVec2 headPos = ImVec2(dpos.x + 10 * sizeScale, dpos.y + 10 * sizeScale); // this is the center of the headpos

                pos = headPos;
                size = headSize;

            } else {
                size = ImVec2(200, 75);
                pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2 + XOffset.Value, ImGui::GetIO().DisplaySize.y / 2 + YOffset.Value);
            }

            for (int i = 0; i < (int)ParticleCount.Value; i++) {
                Particle particle;
                ImVec2 initialPos;
                // Make the initalpos a random position inside of the targethud rect
                initialPos.x = Math::RandomFloat(pos.x, pos.x + size.x);
                initialPos.y = Math::RandomFloat(pos.y, pos.y + size.y);
                particle.pos = initialPos;
                particle.vel = ImVec2(Math::RandomFloat(-400, 400), Math::RandomFloat(-400, 400));
                particle.vel.y *= Velocity.Value;
                particle.vel.x *= Velocity.Value;
                particle.time = 0;
                particle.maxTime = MaxParticleTime.Value;

                float ap = AnimationPerc;
                if (AlwaysShowParticles.Enabled) ap = 1.f;

                if (HealthDependentColor.Enabled) {
                    // Make the particle color yellow if absorption > 0, otherwise red
                    if (LastAbsorption > 0) {
                        particle.color = ImColor(255, 255, 0, (int)((ParticleAlpha.Value * (1 - particle.time / particle.maxTime) * ap)));
                    } else {
                        particle.color = ImColor(255, 0, 0, (int)((ParticleAlpha.Value * (1 - particle.time / particle.maxTime) * ap)));
                    }
                } else {
                    particle.color = ImColor((int)ParticleRed.Value, (int)ParticleGreen.Value, (int)ParticleBlue.Value, (int)((ParticleAlpha.Value * (1 - particle.time / particle.maxTime) * ap)));
                }
                particles.push_back(particle);
            }
        }
    }
}
