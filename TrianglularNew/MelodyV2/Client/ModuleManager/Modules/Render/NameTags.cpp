#include "NameTags.h"
#include "../../../Client.h"

NameTags::NameTags() : Module("NameTags", "Better nam312etags.", Category::RENDER) {
    slider<float>("Opacity", "NULL", ValueType::FLOAT_T, &opacity, 0.f, 1.f);
    boolsetting("Renderlocalplayer", "Render your own nametags", &Renderlocalplayer);
}

void NameTags::onImGuiRender(ImDrawList* d) {
    LocalPlayer* lp = mc.getLocalPlayer();
    if (lp == nullptr) return;
    if (lp->getLevel() == nullptr) return;
    if (!mc.getClientInstance()->minecraftGame->canUseKeys) return;
    static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
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
                Vec4<float> rectPos = Vec4<float>(textPos.x - textPadding * 2.f, textPos.y - textPadding, textPos.x + textWidth + textPadding * 2.f, textPos.y + textHeight + textPadding);
                Vec4<float> underline = Vec4<float>(rectPos.x, rectPos.w - 1.f * textSize, rectPos.z, rectPos.w);

                // Check if the actor's name is in the friends list
                bool isFriend = false;
                for (const auto& friendName : TargetUtils::Friend) {
                    if (name == friendName) {
                        isFriend = true;
                        break;
                    }
                }

                UIColor rectColor = isFriend ? UIColor(0, 255, 0, (int)(255 * opacity)) : UIColor(0, 0, 0, (int)(255 * opacity));
                UIColor underlineColor = isFriend ? UIColor(0, 255, 0, 255) : UIColor(255, 255, 255, 255);
                UIColor textColor = isFriend ? UIColor(0, 255, 0, 255) : UIColor(255, 255, 255, 255);

                ImGuiUtils::fillRectangle(rectPos, rectColor);
                ImGuiUtils::fillRectangle(underline, underlineColor);
                ImGuiUtils::drawText(textPos, name, textColor, textSize, true);
                float armorYOffset = textPos.y + textHeight + textPadding / 2.f;
                float xOffset = 0.0f;
                float xSpacing = 60.0f; 
                for (int i = 0; i < 4; i++) {
                    auto* armorPiece = actor->getArmor(i);
                    if (armorPiece && armorPiece->getitem()) {
                        std::string armorName = std::to_string(armorPiece->getitem() ? (*armorPiece->getitem())->getitemId() : 0);
                        armorName = Utils::sanitize(armorName);
                        float armorWidth = ImGuiUtils::getTextWidth(armorName, textSize);
                        Vec2<float> armorPos = Vec2<float>(textPos.x + textPadding * 2.f + xOffset - armorWidth / 2.f, armorYOffset);
                       // ImGuiUtils::drawText(armorPos, armorName, UIColor(255, 255, 255, 255), textSize * 0.8f, true); this is a placeholder. 
                        xOffset += xSpacing;
                    }
                }

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
            float armorYOffset = textPos.y - textHeight - textPadding / 2.f;
            float xOffset = 0.0f;
            float xSpacing = 60.0f;
            for (int i = 0; i < 4; i++) {
                auto* actor = mc.getLocalPlayer();
                auto* armorPiece = actor->getArmor(i);
                if (armorPiece && armorPiece->getitem()) {
                    std::string armorName = std::to_string(armorPiece->getitem() ? (*armorPiece->getitem())->getitemId() : 0);
                    armorName = Utils::sanitize(armorName);
                    float armorWidth = ImGuiUtils::getTextWidth(armorName, textSize);
                    Vec2<float> armorPos = Vec2<float>(textPos.x + textPadding * 2.f + xOffset - armorWidth / 2.f, armorYOffset);
                   //ImGuiUtils::drawText(armorPos, armorName, UIColor(255, 255, 255, 255), textSize * 0.8f, true); placeholder.
                    xOffset += xSpacing;
                }
            }
        }
    }
}