#pragma once

class Ability {
public:
    // Ability inner types define
    enum class Options : signed char {
        None = 0x0,
        NoSave = 0x1,
        CommandExposed = 0x2,
        PermissionsInterfaceExposed = 0x4,
    };

    enum class Type : signed char {
        Invalid = 0x0,
        Unset = 0x1,
        Bool = 0x2,
        Float = 0x3,
    };

    union Value {
        bool  mBoolVal;
        float mFloatVal;
    };

    Ability::Type    mType;    // this+0x0
    Ability::Value   mValue;   // this+0x4
    Ability::Options mOptions; // this+0x8
};