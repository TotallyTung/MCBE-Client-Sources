#pragma once

class Nametags : public Module
{
public:
    Nametags(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("NameTags", "Render", "Draws peoples nametags above their heads", keybind, enabled)
    {
        registerFloatSetting("Opacity", "Error", &opacity, 0.f, 1.f);
        registerFloatSetting("Radius", "Error", &radius, 0.f, 10.f);
    }

    float opacity = 0.4f;
    float radius = 5.f;
    
    std::string removeBracket(const std::string& input) {
        std::string result;
        bool insideBracket = false;

        for (char c : input) {
            if (c == '[') {
                insideBracket = true;
            }
            else if (c == ']') {
                insideBracket = false;
            }
            else if (!insideBracket) {
                result += c;
            }
        }

        return result;
    }

    void findEntity(Player* entity)
    {
        if (!entity || entity->isLocalPlayer() || entity == Game::GetLocalPlayer() || !Game::GetInstance()->mcGame->canUseMoveKeys())
            return;

        Player* localPlayer = Game::GetLocalPlayer();

        if (entity->isBot() || !localPlayer)
            return;

        Vector3<float> renderPos = entity->getRenderPositionComponent()->renderPos - Vector3 <float>(0, -1.1f, 0);
        float distance = renderPos.distance(Game::GetInstance()->getLevelRender()->getOrigin());

        if (distance > 200)
            return;

        Vector2<float> output1;

        if (!Game::GetInstance()->WorldToScreen(renderPos, output1))
            return;

        if (!entity->getNameTag())
            return;

        std::string user(entity->getNameTag()->getText());
        user = Util::sanitizeName(user);
        user = removeBracket(user);
        if (!user.empty() && user[0] == ' ') {
            user.erase(0, 1); // æ“ª‚Ì‹ó”’‚ğíœ
        }
        float textWidth = ImRenderUtil::getTextWidth(&user, 1.f);
        Vector4<float> rectPos;
        rectPos.x = output1.x - Vector2<float>(textWidth / 1.8, 0).x * 1.f;
        rectPos.y = output1.y - 1.f * 1 * 1.f;
        rectPos.z = output1.x + Vector2<float>(textWidth / 1.8, 0).x * 1.f;
        rectPos.w = output1.y + 20.2f * 1.f;


        ImRenderUtil::fillShadowRectangle(rectPos, UIColor(0, 0, 0), 0.4f, 20.f, 0, 7.f);
        ImRenderUtil::fillRectangle(rectPos, UIColor(0, 0, 0, opacity), opacity, radius);
        ImRenderUtil::drawText(output1 - Vector2<float>(textWidth / 2, 0), &user, UIColor(255, 255, 255), 1.f, 1, true);
    }


    void onEvent(ImGUIRenderEvent* event) override
    {
        auto callback = [this](Player* act) { findEntity(act); };
        getEntities(callback);
    }
};