#pragma once

class TemporalAttributeBuff : public AttributeBuff {
public:
    int32_t mDuration;       // 0x58
    int32_t mLifeTimer;      // 0x5C
    float   mBaseAmount;     // 0x60
    bool    mIsSerializable; // 0x64
public:
    // vIndex: 0
    virtual ~TemporalAttributeBuff();

    // vIndex: 1
    virtual bool isInstantaneous() const;

    // vIndex: 2
    virtual bool isSerializable() const;

    // vIndex: 3
    virtual void setDurationAmplifier(std::shared_ptr<class Amplifier> amplifier);

    // vIndex: 4
    virtual bool shouldBuff() const;

    // vIndex: 5
    virtual bool isComplete() const;
};
static_assert(sizeof(TemporalAttributeBuff) == 0x68);