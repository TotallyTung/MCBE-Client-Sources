#pragma once

enum class AttributeBuffType : int32_t {
    Hunger = 0x0,
    Saturation = 0x1,
    Regeneration = 0x2,
    Heal = 0x3,
    Harm = 0x4,
    Magic = 0x5,
    Wither = 0x6,
    Poison = 0x7,
    FatalPoison = 0x8,
    SelfHeal = 0x9,
    Unknown = 0xA,
    None = 0xB,
};

struct AttributeBuffInfo {
    AttributeBuffType mType;   // 0x0
    size_t            mSource; // 0x8
};
static_assert(sizeof(AttributeBuffInfo) == 0x10);

class AttributeBuff {
public:
    float                      mAmount; // 0x8
    AttributeBuffInfo          mInfo; // 0x10
    std::shared_ptr<Amplifier> mValueAmplifier; // 0x20
    std::shared_ptr<Amplifier> mDurationAmplifier; // 0x30
    float                      mScale; // 0x40
    int32_t                    mAmplification; // 0x44
    uint64_t                   mId; // 0x48
    int32_t                    mOperand; // 0x50
public:
    // vIndex: 0
    virtual ~AttributeBuff();

    // vIndex: 1
    virtual bool isInstantaneous() const = 0;

    // vIndex: 2
    virtual bool isSerializable() const = 0;

    // vIndex: 3
    virtual void setDurationAmplifier(std::shared_ptr<class Amplifier> amplifier);
};