#include "ViewModel.h"

ViewModel::ViewModel() : Module("ViewModel", "Customize yo hand.", Category::RENDER) {
    registerSetting(new EnumSetting("MainHand", "NULL", { "Right", "Left" }, &mainHand, 0));

    registerSetting(new BoolSetting("Translate Hand", "NULL", &translate, true));
    registerSetting(new BoolSetting("Scale Hand", "NULL", &scale, true));
    registerSetting(new BoolSetting("Rotate Hand", "NULL", &rotate, true));

    registerSetting(new SliderSetting<float>("TransX", "NULL", &translateVec.x, 0.f, -2.f, 2.f));
    registerSetting(new SliderSetting<float>("TransY", "NULL", &translateVec.y, 0.f, -2.f, 2.f));
    registerSetting(new SliderSetting<float>("TransZ", "NULL", &translateVec.z, 0.f, -5.f, 2.f));

    registerSetting(new SliderSetting<float>("ScaleX", "NULL", &scaleVec.x, 1.f, -3.f, 3.f));
    registerSetting(new SliderSetting<float>("ScaleY", "NULL", &scaleVec.y, 1.f, -2.f, 2.f));
    registerSetting(new SliderSetting<float>("ScaleZ", "NULL", &scaleVec.z, 1.f, -2.f, 5.f));

    registerSetting(new SliderSetting<float>("RotX", "NULL", &rotateVec.x, 0.f, 0.f, 2.f));
    registerSetting(new SliderSetting<float>("RotY", "NULL", &rotateVec.y, 0.f, 0.f, 2.f));
    registerSetting(new SliderSetting<float>("RotZ", "NULL", &rotateVec.z, 0.f, 0.f, 2.f));

    registerSetting(new SliderSetting<float>("Angle", "Hand rotation angle", &rotAngle, 0.f, -6.3f, 6.3f));

    registerSetting(new BoolSetting("Reset", "Resets all the values to default", &reset, false));
}

void ViewModel::onMatrixRender(glm::mat4* matrix) {
    glm::mat4& matrix2 = *matrix;
    if (reset) {
        translateVec = Vec3<float>(0, 0, 0);
        scaleVec = Vec3<float>(1.f, 1.f, 1.f);
        rotateVec = Vec3<float>(0.1f, 0.f, 0.f);
        rotAngle = 0.f;
        reset = false;
    }
        if (translate) matrix2 = glm::translate<float>(matrix2, glm::vec3(translateVec.x, translateVec.y, translateVec.z));
        if (scale) matrix2 = glm::scale<float>(matrix2, glm::vec3(scaleVec.x - mainHand, scaleVec.y, scaleVec.z));
        if (rotate) matrix2 = glm::rotate<float>(matrix2, rotAngle, glm::vec3(rotateVec.x, rotateVec.y, rotateVec.z));
}