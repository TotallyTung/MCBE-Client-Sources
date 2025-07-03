#pragma once

struct NetworkIdentifierWithSubId {
public:
    NetworkIdentifier mIdentifier{};  // this+0x0
    SubClientId       mSubClientId{}; // this+0xA0
};