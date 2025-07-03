//
// Created by vastrakai on 7/7/2024.
//

#include "LevelRenderer.hpp"

#include <libhat/Access.hpp>
#include <SDK/OffsetProvider.hpp>

glm::vec3* LevelRendererPlayer::getCameraPos()
{
    return &hat::member_at<glm::vec3>(this, OffsetProvider::LevelRendererPlayer_mCameraPos);
}

LevelRendererPlayer* LevelRenderer::getRendererPlayer()
{
    return hat::member_at<LevelRendererPlayer*>(this, OffsetProvider::LevelRenderer_mRendererPlayer);
}
