#pragma once

//
// Created by alteik on 26/10/2024.
//

class JumpCircles : public ModuleBase<JumpCircles>
{
public:

    struct Circle {
        glm::vec3 position;
        float radius;
        ImVec4 color;
        float glowAmount;
        float opacity;
        uint64_t startTime;
    };

    NumberSetting mSize = NumberSetting("Speed", "How fast should the animations be", 0.02f, 0.01f, 0.20f, 0.01f);
    NumberSetting mGlowAmount = NumberSetting("GlowAmount", "How much the circle should glow", 24, 0, 100, 1);
    NumberSetting mOpacity = NumberSetting("Opacity", "The opacity of circle", 0.6f, 0.f, 1.f, 0.01f);
    NumberSetting mTime = NumberSetting("Life time (ms)", "Time to render circle", 9000, 0, 5000, 1);

    JumpCircles() : ModuleBase("JumpCircles", "Render circle when u jumping", ModuleCategory::Visual, 0, false) {

        addSetting(&mSize);
        addSetting(&mGlowAmount);
        addSetting(&mOpacity);
        addSetting(&mTime);

        mNames = {
            {Lowercase, "jumpcircles"},
            {LowercaseSpaced, "jump circles"},
            {Normal, "JumpCircles"},
            {NormalSpaced, "Jump Circles"}
        };
    }

    std::vector<Circle> circles;
    int mTicks = 0;
    uint64_t lastAddTime = 0;
    float ringSpacing = 0.08; // todo make this a number setting

    float toRadians(float deg);
    void addCircle(const glm::vec3& pos, float radius, const ImVec4& color, float opacity, float glowAmount);

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};
