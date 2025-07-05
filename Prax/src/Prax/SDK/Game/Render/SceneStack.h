//
// Created by vastrakai on 1/13/2024.
//


#pragma once

#include <Prax/Utils/System/Memory.h>


class AbstractScene
{
public:
    BUILD_ACCESS_STRING(this, name, 0x28);

    AbstractScene() = default;
};

class SceneStack {
public:
    std::vector<std::unique_ptr<AbstractScene>> scenes = std::vector<std::unique_ptr<AbstractScene>>();

    virtual ~SceneStack();

    AbstractScene* GetScene(int index);
};