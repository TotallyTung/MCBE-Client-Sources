#pragma once

class HungerAttributeDelegate : public AttributeInstanceDelegate {
public:
    // vIndex: 0
    virtual ~HungerAttributeDelegate() = default;

    // vIndex: 1
    virtual void tick();

    // vIndex: 2
    virtual void notify(int64_t type);
};