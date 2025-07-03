#include "TargetHUD.h"
#include <unordered_map>
#include <algorithm> // std::clamp, std::sort
#include <cmath>
#include <vector>    // std::vector

// Actor* -> float (0.0f ile 1.0f arasında renk faktörü)
static std::unordered_map<Actor*, float> colorFactors;

static bool envaledchar(char c) {
    return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string sanitizeBB(const std::string& text) {
    std::string out;
    bool wasValid = true;
    for (char c : text) {
        bool isValid = !envaledchar(c);
        if (wasValid) {
            if (!isValid) {
                wasValid = false;
            }
            else {
                out += c;
            }
        }
        else {
            wasValid = isValid;
        }
    }
    return out;
}

float Size;
float hudOpacity;
int hudPosition;
bool useRounded;
bool useOutline;
bool smoothHurt;

// TargetHUD类的构造函数，初始化模块名称、描述和类别
TargetHUD::TargetHUD() : Module("TargetHUD", "Highlights player/entity", Category::RENDER) {
    registerSetting(new EnumSetting("Pos", "Choose HUD position", { "WorldToScreen", "Hotbar" }, &hudPosition, 0));
    registerSetting(new BoolSetting("Rounded", "Use rounded corners", &useRounded, false));
    registerSetting(new BoolSetting("Outline", "Enable outline for HUD", &useOutline, false));
    registerSetting(new BoolSetting("Smooth Hurt", "Smooth transition for hurt effect", &smoothHurt, true));
    registerSetting(new SliderSetting<float>("Opacity", "Adjust HUD opacity", &hudOpacity, 1.0f, 0.0f, 1.0f));
}

// 存储目标信息的结构体
struct TargetInfo {
    Actor* actor;
    float distance;
};

// 绘制圆角矩形的函数
void drawRoundedRectangle(Vec4<float> rect, UIColor color, float radius) {
    D2D::fillRectangle(Vec4<float>(rect.x + radius, rect.y, rect.z - radius, rect.w), color);
    D2D::fillRectangle(Vec4<float>(rect.x, rect.y + radius, rect.z, rect.w - radius), color);

    D2D::fillCircle(Vec2<float>(rect.x + radius, rect.y + radius), color, radius);
    D2D::fillCircle(Vec2<float>(rect.z - radius, rect.y + radius), color, radius);
    D2D::fillCircle(Vec2<float>(rect.x + radius, rect.w - radius), color, radius);
    D2D::fillCircle(Vec2<float>(rect.z - radius, rect.w - radius), color, radius);
}

// D2D渲染回调函数，用于在游戏界面中渲染目标HUD
void TargetHUD::onD2DRender() {
    LocalPlayer* lp = Game.getLocalPlayer();
    if (lp == nullptr) return;

    std::vector<TargetInfo> targets;

    for (Actor* actor : lp->getlevel()->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(actor))
            continue;

        float distance = lp->getPos().dist(actor->getPos());
        if (distance > 40.0f)
            continue;

        targets.push_back({ actor, distance });
    }

    // 按距离对目标进行排序，并只保留最近的三个目标
    std::sort(targets.begin(), targets.end(), [](const TargetInfo& a, const TargetInfo& b) {
        return a.distance < b.distance;
        });

    if (targets.size() > 3) {
        targets.resize(3); // 如果目标超过三个，则只渲染最近的三个目标
    }

    float textPadding = 10.f;
    float rectPadding = 5.f;

    for (size_t i = 0; i < targets.size(); ++i) {
        Actor* closestActor = targets[i].actor;
        float closestDistance = targets[i].distance;

        Vec2<float> screenPos;
        if (hudPosition == 0) {
            if (!Game.clientInstance->WorldToScreen(closestActor->getPos().add2(0.f, -0.2, 0), screenPos))
                continue;
        }
        else {
            screenPos.x = Game.clientInstance->getguiData()->windowSizeReal.x - 2515 + (i * 150); // 每个目标向右偏移一点
            screenPos.y = Game.clientInstance->getguiData()->windowSizeReal.y - 540;
        }

        std::string actorName = sanitizeBB(*closestActor->getNameTag());
        actorName.resize(14, ' ');

        std::string nameLine = "Name: " + actorName;
        char distStr[32];
        sprintf(distStr, "%.1fm", closestDistance);
        std::string distanceLine = "Distance: " + std::string(distStr);
        std::string hpLine = "HP: " + std::string(/*hpStr*/);

        float textSize = Size;
        float totalTextHeight = (D2D::getTextHeight(nameLine, textSize) * 3) + (rectPadding * 4);
        float maxLineWidth = std::max({ D2D::getTextWidth(nameLine, textSize), D2D::getTextWidth(distanceLine, textSize), D2D::getTextWidth(hpLine, textSize) }) + 20.f;

        Vec4<float> rectPos(screenPos.x - 50.f, screenPos.y - 20.f, screenPos.x + maxLineWidth + 50.f, screenPos.y + totalTextHeight);

        float& colorFactor = colorFactors[closestActor];
        colorFactor += (closestActor->getHurtTime() > 8) ? 0.08f : -0.04f;
        colorFactor = std::clamp(colorFactor, 0.0f, 1.0f);

        int opacityValue = static_cast<int>(hudOpacity * 255);
        UIColor rectColor = (smoothHurt) ? UIColor(static_cast<int>(255 * colorFactor), 0, 0, opacityValue) : (closestActor->getHurtTime() > 8 ? UIColor(255, 0, 0, opacityValue) : UIColor(0, 0, 0, opacityValue));

        if (useRounded) {
            drawRoundedRectangle(rectPos, rectColor, 8.f);
        }
        else {
            D2D::fillRectangle(rectPos, rectColor);
        }

        if (useOutline) {
            D2D::drawRectangle(rectPos, UIColor(255, 255, 255, 255), 2.f);
        }

        Vec2<float> textStartPos(rectPos.x + rectPadding, rectPos.y + rectPadding);
        D2D::drawText(textStartPos, nameLine, UIColor(255, 255, 255, 255), textSize, true);
        D2D::drawText(Vec2<float>(textStartPos.x, textStartPos.y + D2D::getTextHeight(nameLine, textSize)), distanceLine, UIColor(255, 255, 255, 255), textSize, true);
        D2D::drawText(Vec2<float>(textStartPos.x, textStartPos.y + 2 * D2D::getTextHeight(nameLine, textSize)), hpLine, UIColor(255, 255, 255, 255), textSize, true);

        float underlineY = rectPos.w - 3.f;
        D2D::drawLine(Vec2<float>(rectPos.x + rectPadding, underlineY), Vec2<float>(rectPos.z - rectPadding, underlineY), UIColor(255, 255, 255, opacityValue), 3.f);
    }
}
