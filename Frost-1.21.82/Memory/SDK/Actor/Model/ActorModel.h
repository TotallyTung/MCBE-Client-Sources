#pragma once

struct ActorPartModel {
    Vector3<float> mPos;  // this+0x00
    Vector3<float> mRot;  // this+0x0C
    Vector3<float> mSize; // this+0x18
};

struct ActorModel {
    ActorPartModel* mModels[6];

    ActorModel() {
        memset(this, 0, sizeof(ActorModel));
    }

    __forceinline bool isValid() {
        for (int i = 0; i < 6; i++) {
            if (mModels[i] == nullptr) return false;
        }
        return true;
    }
};