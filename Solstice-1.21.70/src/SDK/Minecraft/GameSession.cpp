//
// Created by vastrakai on 6/25/2024.
//

#include "GameSession.hpp"

#include <libhat.hpp>
#include <SDK/OffsetProvider.hpp>

void* GameSession::getEventCallback()
{
    return hat::member_at<void*>(this, OffsetProvider::GameSession_mEventCallback);
}
