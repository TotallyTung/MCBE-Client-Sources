// Created by Tony on 2024-10-10 10:01:54

#include "NameTags.h"
#include "../../../Client.h"

NameTags::NameTags() : Module("NameTags", "Better nametags.", Category::RENDER) {
    addSlider<float>("Opacity", "NULL", ValueType::FLOAT_T, &opacity, 0.f, 1.f);
    addBoolCheck("Renderlocalplayer", "Render your own nametags", &Renderlocalplayer);
}

void NameTags::onImGuiRender(ImDrawList* d) {
    LocalPlayer* lp = mc.getLocalPlayer();
    if (lp == nullptr) return;
    if (lp->getLevel() == nullptr) return;
    if (!mc.getClientInstance()->minecraftGame->canUseKeys) return;
    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
    UIColor mainColor = colorsMod->getColor();

    for (Actor*& actor : lp->getLevel()->getRuntimeActorList()) {
        if (TargetUtils::isTargetValid(actor, false)) {
            Vec2<float> pos;
            if (ImGuiUtils::worldToScreen(actor->getEyePos().add(0.f, 0.75f, 0.f), pos)) {
                float dist = actor->getEyePos().dist(lp->getEyePos());

                float size = fmax(0.65f, 3.f / dist);
                if (size > 2.f) size = 2.f;
                std::string name = *actor->getNameTag();
                name = Utils::sanitize(name);
                float textSize = 1.f * size;
                float textWidth = ImGuiUtils::getTextWidth(name, textSize);
                float textHeight = ImGuiUtils::getTextHeight(textSize);
                float textPadding = 1.f * textSize;
                Vec2<float> textPos = Vec2<float>(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
                Vec4<float> rectPos = Vec4<float>(textPos.x - textPadding * 2.f,
                    textPos.y - textPadding,
                    textPos.x + textWidth + textPadding * 2.f,
                    textPos.y + textHeight + textPadding);
                Vec4<float> underline = Vec4<float>(rectPos.x, rectPos.w - 1.f * textSize, rectPos.z, rectPos.w);

                // Check if the actor's name is in the friends list
                bool isFriend = false;
                for (const auto& friendName : TargetUtils::Friend) {
                    if (name == friendName) {
                        isFriend = true;
                        break;
                    }
                }

                // If the actor is a friend, use green color, otherwise use normal colors
                UIColor rectColor = isFriend ? UIColor(0, 255, 0, (int)(255 * opacity)) : UIColor(0, 0, 0, (int)(255 * opacity));
                UIColor underlineColor = isFriend ? UIColor(0, 255, 0, 255) : UIColor(255, 255, 255, 255);
                UIColor textColor = isFriend ? UIColor(0, 255, 0, 255) : UIColor(255, 255, 255, 255);

                ImGuiUtils::fillRectangle(rectPos, rectColor);
                ImGuiUtils::fillRectangle(underline, underlineColor);
                ImGuiUtils::drawText(textPos, name, textColor, textSize, true);
            }
        }
    }

    if (Renderlocalplayer) {
        Vec2<float> pos;
        if (ImGuiUtils::worldToScreen(mc.getLocalPlayer()->getEyePos().add(0.f, 0.75f, 0.f), pos)) {
            float dist = mc.getLocalPlayer()->getEyePos().dist(lp->getEyePos());

            float size = fmax(0.65f, 3.f / dist);
            if (size > 2.f) size = 2.f;
            std::string name = *mc.getLocalPlayer()->getNameTag();
            name = Utils::sanitize(name);
            float textSize = 1.f * size;
            float textWidth = ImGuiUtils::getTextWidth(name, textSize);
            float textHeight = ImGuiUtils::getTextHeight(textSize);
            float textPadding = 1.f * textSize;
            Vec2<float> textPos = Vec2<float>(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
            Vec4<float> rectPos = Vec4<float>(textPos.x - textPadding * 2.f,
                textPos.y - textPadding,
                textPos.x + textWidth + textPadding * 2.f,
                textPos.y + textHeight + textPadding);
            Vec4<float> underline = Vec4<float>(rectPos.x, rectPos.w - 1.f * textSize, rectPos.z, rectPos.w);

            ImGuiUtils::fillRectangle(rectPos, UIColor(0, 0, 0, (int)(255 * opacity)));
            ImGuiUtils::fillRectangle(underline, UIColor(255, 255, 255, 255));
            ImGuiUtils::drawText(textPos, name, UIColor(255, 255, 255, 255), textSize, true);
        }
    }
}
