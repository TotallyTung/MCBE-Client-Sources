//
// Created by vastrakai on 9/14/2024.
//

#include "ViewModel.hpp"

#include <Features/Events/BobHurtEvent.hpp>

void ViewModel::onEnable()
{
    gFeatureManager->mDispatcher->listen<BobHurtEvent, &ViewModel::onBobHurtEvent, nes::event_priority::FIRST>(this);
}

void ViewModel::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BobHurtEvent, &ViewModel::onBobHurtEvent>(this);
}

void ViewModel::onBobHurtEvent(BobHurtEvent& event)
{
    float x = mX.mValue;
    float y = mY.mValue;
    float z = mZ.mValue;

    glm::mat4 matrix = *event.mMatrix;
    matrix = glm::translate(matrix, glm::vec3(x, y, z));
    *event.mMatrix = matrix;
}
