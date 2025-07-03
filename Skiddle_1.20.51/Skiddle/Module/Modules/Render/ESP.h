#pragma once

class ESP : public Module
{
public:
    ESP(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("ESP", "Render", "Draw basic boxes around entities", keybind, enabled)
    {
        registerEnumSetting("Mode", "The mode of ESP visual", { "2D" }, &mode);
        registerFloatSetting("Thickness", "The thickness of each pixel", &thickness, 1.f, 4.f);
    }

    float thickness = 1;
    int mode = 0;

    void drawEntity(Player* entity)
    {
        if (!entity || entity->isLocalPlayer() || entity == Game::GetLocalPlayer() || !Game::GetInstance()->mcGame->canUseMoveKeys())
            return;

        AABBShapeComponent* shape = entity->try_get<AABBShapeComponent>();
        if (!shape || IsBadReadPtr(shape, sizeof(AABBShapeComponent))) return;

        Vector3<float> entDims = { shape->r_Hitbox.x, shape->r_Hitbox.y, shape->r_Hitbox.x };

        if (entDims.x != 0.6f || entDims.y != 1.8f)
            return;

        switch (mode) {
        case 0: // 2D
            Vector3<float> entLower = shape->r_Pos_Lower;
            Vector3<float> entUpper = shape->r_Pos_Upper;
            Vector3<float> entPos = entity->GetPosition();

            Vector3<float> origin = Game::GetInstance()->getLevelRender()->getOrigin();
            float dist = origin.distance(entPos);

            Vector2<float> output1, output2;
            if (!Game::GetInstance()->WorldToScreen(entLower, output1) || !Game::GetInstance()->WorldToScreen(entUpper, output2)) return;

            std::vector<Vector3<float>> aabbArr;
            for (float x : { entLower.x, entUpper.x })
            {
                for (float y : { entLower.y, entUpper.y })
                {
                    for (float z : { entLower.z, entUpper.z })
                    {
                        aabbArr.push_back({ x, y, z });
                    }
                }
            }

            std::vector<Vector2<float>> scrPoints;
            for (int i = 0; i < aabbArr.size(); i++)
            {
                Vector2<float> scrPoint;
                if (Game::GetInstance()->WorldToScreen(aabbArr[i], scrPoint))
                    scrPoints.push_back(scrPoint);
            }

            if (scrPoints.empty())
                return;

            Vector4<float> boundingRect = { scrPoints[0].x, scrPoints[0].y, scrPoints[0].x, scrPoints[0].y };
            for (const auto& point : scrPoints)
            {
                boundingRect.x = std::min(boundingRect.x, point.x);
                boundingRect.y = std::min(boundingRect.y, point.y);
                boundingRect.z = std::max(boundingRect.z, point.x);
                boundingRect.w = std::max(boundingRect.w, point.y);
            }

            float thickness2 = fmax(1.f, 1.f / std::max(1.f, origin.distance(entPos)));
            thickness2 *= thickness;
            int index = 0;
            UIColor arrayColor = ColorUtil::getClientColor(2, 1, 1, index * 80);
            ImRenderUtil::drawRoundRect(boundingRect, 0, 0.f, UIColor(0, 0, 0), 0.75f, thickness2 + 2.f);
            ImRenderUtil::drawRoundRect(boundingRect, 0, 0.f, UIColor(arrayColor), 1.f, thickness2);
            break;
        }
    }


    void onEvent(ImGUIRenderEvent* event) override
    {
        auto callback = [this](Player* act) { drawEntity(act); };
        getEntities(callback);
    }
};
