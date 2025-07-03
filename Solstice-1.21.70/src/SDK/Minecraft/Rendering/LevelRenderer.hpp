#pragma once
#include <Utils/MemUtils.hpp>


//
// Created by vastrakai on 7/7/2024.
//

class LevelRendererPlayer {
public:
    glm::vec3* getCameraPos();
};

class LevelRenderer {
public:
    //CLASS_FIELD(LevelRendererPlayer*, mLevelRendererPlayer, 0x308);
    LevelRendererPlayer* getRendererPlayer();
};