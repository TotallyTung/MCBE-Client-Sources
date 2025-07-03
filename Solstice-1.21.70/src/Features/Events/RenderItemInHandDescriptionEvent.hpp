//
// Created by vastrakai on 10/19/2024.
//

#pragma once

#include "Event.hpp"

class RenderItemInHandDescriptionEvent : public CancelableEvent {
public:
    RenderItemInHandDescriptionEvent(class RenderItemInHandDescription* _this, void* renderObject, void* itemFlags, void* material, void* glintTexture, void* worldMatrix, bool isDrawingUI, void* globalConstantBuffers)
        : mThis(_this), mRenderObject(renderObject), mItemFlags(itemFlags), mMaterial(material), mGlintTexture(glintTexture), mWorldMatrix(worldMatrix), mIsDrawingUI(isDrawingUI), mGlobalConstantBuffers(globalConstantBuffers) {}

    RenderItemInHandDescription* mThis;
    void* mRenderObject;
    void* mItemFlags;
    void* mMaterial;
    void* mGlintTexture;
    void* mWorldMatrix;
    bool mIsDrawingUI;
    void* mGlobalConstantBuffers;
};
