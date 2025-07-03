#pragma once

class BaseTickEvent : public Event
{
public:
    BaseTickEvent(Actor* _actor, Level* _level, GameMode* _gamemode)
    {
        mActor = _actor;
        mLevel = _level;
        mGameMode = _gamemode;
    }

    EventType getType() const override { return EventType::BaseTick; }
    Actor* mActor;
    Level* mLevel;
    GameMode* mGameMode;
};