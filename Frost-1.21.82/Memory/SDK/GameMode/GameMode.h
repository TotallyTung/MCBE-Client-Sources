#pragma once

class Player;

#include <functional>

class GameMode
{
public:
    Player&                               mPlayer;                       // this+0x8
    Vector3<int>                          mDestroyBlockPos;              // this+0x10
    unsigned char                         mDestroyBlockFace;             // this+0x1C
    float                                 mOldDestroyProgress;           // this+0x20
    float                                 mDestroyProgress;              // this+0x24
    double                                mLastDestroyTime;              // this+0x28
    float                                 mDistanceTravelled;            // this+0x30
    Vector3<float>                        mPlayerLastPosition;           // this+0x34
    Vector3<int>                          mLastBuiltBlockPosition;       // this+0x40
    bool                                  mLastBuildBlockWasInteractive; // this+0x4C
    bool                                  mLastBuildBlockWasSnappable;   // this+0x4D
    float                                 mMinPlayerSpeed;               // this+0x50
    int                                   mContinueBreakBlockCount;      // this+0x54
    std::chrono::steady_clock::time_point mLastBuildTime;                // this+0x58
    std::chrono::steady_clock::time_point mNoDestroyUntil;               // this+0x60
    std::chrono::steady_clock::time_point mNoDestroySoundUntil;          // this+0x68
    std::chrono::milliseconds             creativeDestructionTickDelay;  // this+0x70
    std::chrono::milliseconds             buildingTickDelay;             // this+0x78
    std::chrono::milliseconds             destroySoundDelay;             // this+0x80
    bool                                  mHasBuildDirection;            // this+0x88
    bool                                  mHasLastBuiltPosition;         // this+0x89
    unsigned char                         mContinueFacing;               // this+0x8A
    Vector3<int>                          mBuildDirection;               // this+0x8C
    Vector3<int>                          mNextBuildPos;                 // this+0x98
    //std::unique_ptr<IGameModeTimer>       mTimer;                        // this+0xA8
    //std::unique_ptr<IGameModeMessenger>   mMessenger;                    // this+0xB0

    //CLASS_FIELD(float, lastBreakProgress, 0x20);
    //CLASS_FIELD(float, BreakProgress, 0x24);
private:
    virtual void Destructor(); // 0 // GameMode Destructor
public:
    virtual __int64 startDestroyBlock(Vector3<int> const& pos, unsigned char blockSide, bool& isDestroyedOut); // 1
    virtual __int64 destroyBlock(const Vector3<int>&, unsigned char blockSide); // 2
    virtual __int64 continueDestroyBlock(Vector3<int> const& a1, unsigned char a2, Vector3<float> const& a3, bool& a4); // 3
    virtual __int64 stopDestroyBlock(Vector3<int> const&); // 4
    virtual __int64 startBuildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 5
    virtual __int64 buildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 6
    virtual __int64 continueBuildBlock(Vector3<int> const& a1, unsigned char a2); // 7
    virtual __int64 stopBuildBlock(); // 8
    virtual __int64 tick(); // 9
    virtual __int64 getPickRange(InputMode const& a1, bool a2); // 10
    virtual __int64 useItem(class ItemStack& a1); // 11
    virtual __int64 useItemOn(ItemStack& a1, Vector3<int> const& a2, unsigned char a3, Vector3<float> const& a4, class Block const* a5); // 12
    virtual __int64 interact(Actor* a1, Vector3<float> const& a2); // 13
    virtual __int64 attack(Actor&); // 14
    virtual __int64 releaseUsingItem(); // 15
    virtual void setTrialMode(bool a1); // 16
    virtual void isInTrialMode(); // 17
    virtual __int64 registerUpsellScreenCallback(std::function<void> a3); // 18

    float getDestroyRate(const Block& block) {
        return Memory::CallFunc<float, GameMode*, const Block&>(SigManager::GameMode_getDestroyRate, this, block);
    }

    void baseUseItem(ItemStack& a1) {
        Memory::CallFunc<void, GameMode*, ItemStack&>(SigManager::GameMode_baseUseItem, this, a1);
    }
};