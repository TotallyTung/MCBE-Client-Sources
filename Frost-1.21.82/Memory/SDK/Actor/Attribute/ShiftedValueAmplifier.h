#pragma once

class ShiftedValueAmplifier : public Amplifier {
public:
    // vIndex: 0
    virtual ~ShiftedValueAmplifier() = default;

    // vIndex: 1
    virtual float getAmount(int amplification, float scale) const;
};