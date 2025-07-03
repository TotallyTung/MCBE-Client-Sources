#pragma once

class ShiftedDurationAmplifier : public Amplifier {
public:
    // vIndex: 0
    virtual ~ShiftedDurationAmplifier() = default;

    // vIndex: 2
    virtual bool shouldBuff(int remainingDuration, int amplification) const;

    // vIndex: 3
    virtual int getTickInterval(int amplification) const;
};