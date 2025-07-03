//
// Created by vastrakai on 8/14/2024.
//

#pragma once

enum class ServerAuthMovementMode : signed char {
    ClientAuthoritative           = 0x0,
    ServerAuthoritative           = 0x1,
    ServerAuthoritativeWithRewind = 0x2,
};

struct SyncedPlayerMovementSettings
{
public:
    ServerAuthMovementMode AuthorityMode;
    int                    mRewindHistorySize;
    bool                   ServerAuthBlockBreaking;
};