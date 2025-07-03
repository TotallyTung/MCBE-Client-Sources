#pragma once

class HealthAttributeDelegate : public AttributeInstanceDelegate {
public:
    // vIndex: 0
    virtual ~HealthAttributeDelegate() = default;

    // vIndex: 1
    virtual void tick();

    // vIndex: 3
    virtual bool willChange(float oldValue, float newValue, class AttributeBuff const& buff);

    // vIndex: 4
    virtual float change(float oldValue, float newValue, class AttributeBuff const& buff);

    // vIndex: 5
    virtual float getBuffValueWithModifiers(class AttributeBuff const& buff) const;
};