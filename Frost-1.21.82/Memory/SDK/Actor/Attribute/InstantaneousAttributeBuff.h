#pragma once

class InstantaneousAttributeBuff : public AttributeBuff {
public:
    // vIndex: 0
    virtual ~InstantaneousAttributeBuff();

    // vIndex: 1
    virtual bool isInstantaneous() const;

    // vIndex: 2
    virtual bool isSerializable() const;
};