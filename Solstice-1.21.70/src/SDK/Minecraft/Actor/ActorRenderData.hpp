#pragma once
//
// Created by jcazm on 7/27/2024.
//

struct ActorRenderData {
    class Actor* mActor;
    int64_t* mData; // e. g. ItemStack if the actor is an ItemActor
    glm::vec3 mPosition;
    glm::vec2 mRotation;
    glm::vec2 mHeadRot;
    bool mGlint;
    bool mIgnoreLighting;
    bool mIsInUI;
    float mDeltaTime;
    int mModelObjId;
    float mModelSize;

    PAD(0x10);
};
