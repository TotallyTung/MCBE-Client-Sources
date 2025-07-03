#pragma once

class AttributeModifier {
public:
    float                      mAmount;
    AttributeModifierOperation mOperation;
    AttributeOperands          mOperand;
    std::string                mName;
    mce::UUID                  mId;
    bool                       mSerialize;
};
