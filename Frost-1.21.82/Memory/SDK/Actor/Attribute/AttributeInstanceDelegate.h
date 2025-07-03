#pragma once

class AttributeInstanceDelegate {
public:
    AttributeInstanceHandle mAttributeHandle; // 0x8
public:
    // vIndex: 0
    virtual ~AttributeInstanceDelegate() = default;

    // vIndex: 1
    virtual void tick();

    // vIndex: 2
    virtual void notify(int64_t type);

    // vIndex: 3
    virtual bool willChange(float oldValue, float newValue, class AttributeBuff const& buff);

    // vIndex: 4
    virtual float change(float oldValue, float newValue, class AttributeBuff const& buff);

    // vIndex: 5
    virtual float getBuffValueWithModifiers(class AttributeBuff const& buff) const;
};
static_assert(sizeof(AttributeInstanceDelegate) == 0x18);