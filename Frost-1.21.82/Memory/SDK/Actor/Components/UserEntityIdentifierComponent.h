#pragma once

class UserEntityIdentifierComponent {
public:
    class NetworkIdentifier            mNetworkId;   // this+0x0
    ::SubClientId                      mClientSubId; // this+0xA0
    mce::UUID                          mClientUUID;  // this+0xA8
    std::string                        mPlayFabId;   // this+0xB8
    std::unique_ptr<class Certificate> mCertificate; // this+0xD8
};