//
// Created by Flash on 10/19/2023.
//

#include <imgui_internal.h>
#include "ImRotateUtil.h"

#define PI       3.14159265358979323846

ImRotateUtil::ImRotateUtil(ImDrawList *drawList) {
    rotationStartIndex = 0.f;
    this->drawList = drawList;
}

void ImRotateUtil::StartRotation() {
    rotationStartIndex = drawList->VtxBuffer.Size;
}

ImVec2 ImRotateUtil::GetRotationCenter() {
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

    const auto& buf = drawList->VtxBuffer;
    for (int i = rotationStartIndex; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
}

void ImRotateUtil::EndRotation(float rad, ImVec2 center) {
    rad += PI * 0.5f;
    float s = std::sin(rad), c = std::cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = drawList->VtxBuffer;

    for (int i = rotationStartIndex; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}
