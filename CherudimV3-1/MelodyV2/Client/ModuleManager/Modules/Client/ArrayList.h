#pragma once
#include "../Module.h"

class Particle {
public:
    float x, y;
    float vx, vy;

    Particle(float startX, float startY);
    void update(const Vec4<float>& rectPos);
};

class ArrayList : public Module {
public:
    bool bottom = false;
    float size = 1.0f;
    float bgOpacity = 1.0f;
    float spacing = 0.0f;
    int arraymod = 0;
    ArrayList();
    void onImGuiRender(ImDrawList* drawlist) override;

    void handleMouseClick(const Vec2<float>& mousePos);
    std::vector<Particle> particles;
    void drawGlowingRectangle(ImDrawList* drawlist, const Vec4<float>& rectPos, const UIColor& color, float opacity);
};

bool sortByLength(Module* lhs, Module* rhs);