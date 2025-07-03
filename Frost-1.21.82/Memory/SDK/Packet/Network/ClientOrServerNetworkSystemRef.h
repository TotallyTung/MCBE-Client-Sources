#pragma once

#include <variant>

class ClientNetworkSystem {

};

class ServerNetworkSystem {

};

class ClientOrServerNetworkSystemRef {
public:
    using ClientRefT = std::reference_wrapper<ClientNetworkSystem>;
    using ServerRefT = std::reference_wrapper<ServerNetworkSystem>;

    std::variant<ClientRefT, ServerRefT> ref;

    ClientNetworkSystem& toClientNetworkSystem() { return std::get<ClientRefT>(ref).get(); }

public:
    class ServerNetworkSystem& toServerNetworkSystem() {}
};