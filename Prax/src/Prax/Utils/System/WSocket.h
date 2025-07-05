//
// Created by Flash on 7/22/2023.
//

#pragma once

#include <winsock2.h>

class WSocket {
public:
    static SOCKET Socket;
    static WSAData WSAData;

    static HRESULT Init(PCSTR port = "2543");
    static void Receive();
    static void Shutdown();
};