#include "Client/Manager/Client.h"

Client* client = nullptr; // Declare a pointer to Client outside the init function

void cleanup() {
    // Ensure that the Client object is deleted when it's no longer needed
    if (client) {
        delete client;
        client = nullptr; // Set the pointer to nullptr to avoid double deletion
    }
}

void init(HINSTANCE hInstance) {
    client = new Client("Air");
    // It's essential to ensure that cleanup is called when you're done with the Client object.
    // In a real application, you should determine when to call cleanup based on your program's logic.
    // For this example, I'll call it immediately.
    cleanup();
    ExitThread(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, hModule, 0, 0);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // Ensure cleanup is called when the DLL is unloaded
        cleanup();
    }
    return TRUE;
}