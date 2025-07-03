//
// Created by vastrakai on 7/25/2024.
//

#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <SDK/OffsetProvider.hpp>

#include "glm/gtx/rotate_vector.hpp"

struct ActorPartModel {
    glm::vec3 mPos;
    glm::vec3 mRot;
    glm::vec3 mSize;
// example: [21:37:08.432] [info] Bone: rightarm, pos: (-5, -2, 0), size: (1, 1, 1), rot: (-90, 0, 5.4537144)
};

struct Bone
{
    CLASS_FIELD(std::string, mBoneStr, 0x10);

    ActorPartModel* getActorPartModel()
    {
        return reinterpret_cast<ActorPartModel*>(reinterpret_cast<uintptr_t>(this + 0xEC));
    }
};

class ActorModel
{
public:
    std::map<int, ActorPartModel*> mModels;

    ActorModel() = default;
};

