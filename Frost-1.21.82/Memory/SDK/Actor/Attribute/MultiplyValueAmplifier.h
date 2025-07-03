#pragma once

class MultiplyValueAmplifier : public Amplifier {
public:
    // vIndex: 0
    virtual ~MultiplyValueAmplifier() = default;

    // vIndex: 1
    virtual float getAmount(int amplification, float scale) const;
};