#pragma once

class Attribute;

class AttributeInstance {
public:
    Attribute const*                             mAttribute;
    std::vector<::AttributeModifier>             mModifierList;
    std::vector<::TemporalAttributeBuff>         mTemporalBuffs;
    std::vector<::AttributeInstanceHandle>       mListeners;
    std::shared_ptr<::AttributeInstanceDelegate> mDelegate;

    union {
        float mDefaultValues[3];
        struct {
            float mDefaultMinValue;
            float mDefaultMaxValue;
            float mDefaultValue;
        };
    };

    union {
        float mCurrentValues[3];
        struct {
            float mCurrentMinValue;
            float mCurrentMaxValue;
            float mCurrentValue;
        };
    };
public:

    // vIndex: 0
    virtual ~AttributeInstance() {};

    // vIndex: 1
    virtual void tick() {};
};
static_assert(sizeof(AttributeInstance) == 0x80, "AttributeInstance size is not correct");