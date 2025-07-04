#include "ConicalHat.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <future>
#include <vector>

ConicalHat::ConicalHat() : Module("ConicalHat", "Render conical hat above your head", Category::RENDER) {
    addColorPicker("Color", "NULL", &chColor);
    addBoolCheck("Rainbow", "NULL", &rainbow);
}

void ConicalHat::onLevelRender() {
    if (RenderUtils::screenContext3D == nullptr || RenderUtils::blendMaterial == nullptr)
        return;

    Tessellator* tess3D = RenderUtils::getTessellator3D();
    LocalPlayer* lp = mc.getLocalPlayer();

    Vec3<float> lpPos = lp->getEyePos();
    if (mc.cameraPerspectiveMode == 0)
        lpPos = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;

    Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;

    std::vector<Vec3<float>> pointsList;

    auto calculatePoints = [&lpPos, &origin](int i) {
        float calcYaw = (i + 90.f) * glm::pi<float>() / 180.f;
        float x = cos(calcYaw) * 0.55f;
        float z = sin(calcYaw) * 0.55f;
        return lpPos.add(x, 0.25f, z).sub(origin);
        };

    std::vector<std::future<Vec3<float>>> futures;

    for (int i = 0; i < 360; i += 5) {
        futures.push_back(std::async(std::launch::async, calculatePoints, i));
    }

    for (auto& future : futures) {
        pointsList.push_back(future.get());
    }

    Vec3<float> center = lpPos.add(0.f, 0.5f, 0.f).sub(origin);

    UIColor mainColor = chColor;

    for (int i = 0; i < pointsList.size(); i++) {
        if (rainbow)
            mainColor = ColorUtils::getRainbowColor(3.f, 1.f, 1.f, static_cast<long>(i * 42));

        int next = (i + 1) % pointsList.size();

        MC_Color color = mainColor.toMC_Color();
        color.a = 0.4f;
        RenderUtils::setColor(color);

        tess3D->begin(VertextFormat::TRIANGLE_STRIP, 3);
        tess3D->vertex(pointsList[i].x, pointsList[i].y, pointsList[i].z);
        tess3D->vertex(center.x, center.y, center.z);
        tess3D->vertex(pointsList[next].x, pointsList[next].y, pointsList[next].z);
        tess3D->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);

        tess3D->begin(VertextFormat::TRIANGLE_STRIP, 3);
        tess3D->vertex(pointsList[next].x, pointsList[next].y, pointsList[next].z);
        tess3D->vertex(center.x, center.y, center.z);
        tess3D->vertex(pointsList[i].x, pointsList[i].y, pointsList[i].z);
        tess3D->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);

        color.a = 0.75f;
        RenderUtils::setColor(color);

        tess3D->begin(VertextFormat::LINE_STRIP, 2);
        tess3D->vertex(pointsList[i].x, pointsList[i].y, pointsList[i].z);
        tess3D->vertex(pointsList[next].x, pointsList[next].y, pointsList[next].z);
        tess3D->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
    }
}
