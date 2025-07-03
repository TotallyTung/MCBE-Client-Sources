#pragma once

class Amplifier {
public:
    // vIndex: 0
    virtual ~Amplifier() = default;

    // vIndex: 1
    virtual float getAmount(int amplification, float scale) const;

    // vIndex: 2
    virtual bool shouldBuff(int remainingDuration, int amplification) const;

    // vIndex: 3
    virtual int getTickInterval(int amplification) const;
};