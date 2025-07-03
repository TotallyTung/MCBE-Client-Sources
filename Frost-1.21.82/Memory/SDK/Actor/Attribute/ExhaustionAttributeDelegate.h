#pragma once

class ExhaustionAttributeDelegate : public AttributeInstanceDelegate {
public:
    // vIndex: 0
    virtual ~ExhaustionAttributeDelegate() = default;

    // vIndex: 1
    virtual void tick();
};