#include "CrystalChams.h"
#include "../../../Client.h"
bool ClientColor = true;
float speed1 = 0.0f;
float wirewidth = 0.0f;
float rangedayoomg = 0.0f;
float alpha = 0.0f;
float alpha1 = 0.0f;
bool wireframe = false;
UIColor mainColor1 = UIColor(255, 255, 255, 100);
UIColor wireColor1 = UIColor(255, 255, 255, 255);
CrystalChams::CrystalChams() : Module("CrystalChams", "CrystalChams+ 1 by Rusher4039!", Category::RENDER) {
    slider<float>("Range", "NULL", ValueType::FLOAT_T, &rangedayoomg, 0, 30);
    slider<float>("Speed", "NULL", ValueType::FLOAT_T, &speed1, 0.f, 10.0f);
    slider<float>("Wireframe Width", "NULL", ValueType::FLOAT_T, &wirewidth, 0.1f, 1.0f);
    slider<float>("Main Alpha", "NULL", ValueType::FLOAT_T, &alpha, 0.1f, 1.0f);
    slider<float>("Wireframe Alpha", "NULL", ValueType::FLOAT_T, &alpha1, 0.1f, 1.0f);
    boolsetting("Sync", "Sync colors", &ClientColor);
    boolsetting("Wireframe", "Outline", &wireframe);
    colorsetting("Color", "NULL", &mainColor1);
    colorsetting("Wire Color", "NULL", &wireColor1);
}

void CrystalChams::onRender(MinecraftUIRenderContext* renderCtx) {
    auto player = mc.getLocalPlayer();
    if (player == nullptr)
        return;

    auto level = player->getLevel();
    if (level == nullptr)
        return;

    static ClientManager* colorsMod = (ClientManager*)client->moduleMgr->getModule("ClientManager");
    UIColor mainColor(colorsMod->getColor());
    mainColor.r = (int)(mainColor.r);
    mainColor.g = (int)(mainColor.g);
    mainColor.b = (int)(mainColor.b);

    for (Actor* ent : level->getRuntimeActorList()) {
        float distance = player->getPosition()->dist(*ent->getPosition());
        if (distance > rangedayoomg)
            continue;

        if (ent->getEntityTypeId() == 71) { 
            Vec3 lower = *ent->getPosition();
            float sizeX = 0.5f, sizeY = 0.5f, sizeZ = 0.5f;
            Vec3 renderPosOffset(-0.5f, 0.25f, -0.5f);
            lower = lower.add(renderPosOffset);
            Vec3 upper = lower.add(Vec3(sizeX * 2, sizeY * 2, sizeZ * 2));

            static float rotationAngleX = 0.0f, rotationAngleY = 0.0f, rotationAngleZ = 0.0f;
            float deltaTime = speed1 / 60.0f;
            rotationAngleX += speed1 * deltaTime;
            rotationAngleY += speed1 * deltaTime;
            rotationAngleZ -= speed1 * deltaTime;

            Vec3 rotationAxisX(1.f, 0.5f, 0.5f);
            Vec3 rotationAxisY(0.5f, 1.f, 0.5f);
            Vec3 rotationAxisZ(0.5f, 0.5f, 1.f);
            float maxDistance = rangedayoomg; 
            float minWireWidth = 0.5f; 
            float maxWireWidth = wirewidth;

            float factor = (distance / maxDistance);
            factor = factor * factor; 
            float dynamicWireWidth = maxWireWidth - (factor * (maxWireWidth - minWireWidth));
            dynamicWireWidth = std::clamp(dynamicWireWidth, minWireWidth, maxWireWidth); 
            if (ClientColor) {
                RenderUtils::setColor(mainColor.r / 255.f, mainColor.g / 255.f, mainColor.b / 255.f, alpha);
            }
            else {
                RenderUtils::setColor(mainColor1.r / 255.f, mainColor1.g / 255.f, mainColor1.b / 255.f, alpha);
            }
            RenderUtils::drawBoxTest(lower, upper, 2.0f, true, 2, rotationAngleX, rotationAxisX);
            if (wireframe) {
                MC_Color outlineColor;
                if (ClientColor) {
                    outlineColor = MC_Color(mainColor.r, mainColor.g, mainColor.b, static_cast<int>(alpha1 * 255.f));
                }
                else {
                    outlineColor = MC_Color(wireColor1.r, wireColor1.g, wireColor1.b, static_cast<int>(alpha1 * 255.f));
                }
                RenderUtils::drawBoxOutline(lower, upper, dynamicWireWidth, outlineColor, 2, rotationAngleX, rotationAxisX);
            }
            if (ClientColor) {
                RenderUtils::setColor(mainColor.r / 255.f, mainColor.g / 255.f, mainColor.b / 255.f, alpha);
            }
            else {
                RenderUtils::setColor(mainColor1.r / 255.f, mainColor1.g / 255.f, mainColor1.b / 255.f, alpha);
            }
            RenderUtils::drawBoxTest(lower, upper, 2.0f, true, 2, rotationAngleZ, rotationAxisZ);
            if (wireframe) {
                MC_Color outlineColor;
                if (ClientColor) {
                    outlineColor = MC_Color(mainColor.r, mainColor.g, mainColor.b, static_cast<int>(alpha1 * 255.f));
                }
                else {
                    outlineColor = MC_Color(wireColor1.r, wireColor1.g, wireColor1.b, static_cast<int>(alpha1 * 255.f));
                }
                RenderUtils::drawBoxOutline(lower, upper, dynamicWireWidth, outlineColor, 2, rotationAngleZ, rotationAxisZ);
            }
        }
    }
}
