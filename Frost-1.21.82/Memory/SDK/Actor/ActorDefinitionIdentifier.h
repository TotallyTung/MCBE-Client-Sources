#pragma once

struct ActorDefinitionIdentifier {
public:
    std::string  mNamespace;     // 0x0
    std::string  mIdentifier;    // 0x20
    std::string  mInitEvent;     // 0x40
    std::string  mFullName;      // 0x60
    HashedString mCanonicalName; // 0x80
};
//static_assert(sizeof(ActorDefinitionIdentifier) == 0xB0);