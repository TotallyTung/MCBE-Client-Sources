//
// Created by Flash on 10/19/2023.
//


#pragma once

#include <imgui.h>

class ImRotateUtil {
public:
    ImRotateUtil(ImDrawList* drawList);

    void StartRotation();

    ImVec2 GetRotationCenter();

    void EndRotation(float rad, ImVec2 center);
private:
    int rotationStartIndex = 0;
    ImDrawList* drawList;
};