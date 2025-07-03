#include "IRC.h"
#include "../ModuleManager.h"
IRC::IRC() : IModule(0, Category::CLIENT, "Internet Relay Chat") {
}

IRC::~IRC() {
}

const char* IRC::getModuleName() {
    return "IRC";
}
using namespace std;

const char* SERVER_URL = "ws://147.185.221.19:45372/irc";
const char* SEC_WEBOCKET_KEY = "258EAFA5D617B8D121B7F916F8676710";

void IRC::SendToServer(HINTERNET hSession, const string& message) {
    string frameData = "\x81\x00" + message;
    int messageLength = message.length();
    frameData[1] = (char)(messageLength & 0xFF);
    DWORD bytesSent = 0;
    if (!WinHttpWriteData(hSession, frameData.c_str(), frameData.length(), &bytesSent)) {
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Failed to send message", RED, WHITE);
        return;
    }
}

string IRC::GetToClient(HINTERNET hSession) {
    char header[2];
    DWORD bytesRead = 0;
    if (!WinHttpReadData(hSession, header, sizeof(header), &bytesRead) || bytesRead != sizeof(header)) {
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Failed to read message header", RED, WHITE);
        return "";
    }

    bool fin = (header[0] & 0x80) != 0;
    int opcode = header[0] & 0x0F;
    if (opcode != 1) { // Check for text frame (opcode 1)
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Invalid message opcode: %i", RED, WHITE, opcode);
        return "";
    }

    int payloadLength = (header[1] & 0x7F);
    if (payloadLength > 125) { // Extended length (not implemented here for simplicity)
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Extended payload length not supported yet", RED, WHITE);
        return "";
    }

    char messageBuffer[1024];
    if (!WinHttpReadData(hSession, messageBuffer, payloadLength, &bytesRead) || bytesRead != payloadLength) {
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Failed to read message payload", RED, WHITE);
        return "";
    }

    return string(messageBuffer, bytesRead);
}
void IRC::onTick(GameMode*) {
    HINTERNET hSession = WinHttpOpen(0, 0x00000003, NULL, (LPCWSTR)SERVER_URL, INTERNET_DEFAULT_HTTP_PORT);
    if (!hSession) {
        Game.getClientInstance()->getGuiData()->displayClientMessageF("[%sIRC%s] ERROR: Failed to create WinHTTP session", RED, WHITE);
        this->setEnabled(false);
        return;
    }

    string message = "Hello from WinHTTP client!";
    string receivedMessage = GetToClient(hSession);
    if (!receivedMessage.empty())
        Game.getGuiData()->displayClientMessageF("[%sIRC%s] %", BLUE, WHITE, receivedMessage);
    else
        return;
}