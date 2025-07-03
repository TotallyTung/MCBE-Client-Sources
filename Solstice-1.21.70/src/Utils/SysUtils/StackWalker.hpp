#pragma once
//
// Created by vastrakai on 7/7/2024.
//

#include <windows.h>
#include <vector>
#include <string>

class StackWalker {
public:
    StackWalker();
    ~StackWalker();
    std::vector<std::string> ShowCallstack(HANDLE hThread, PCONTEXT pContext);
private:
    void LoadModuleSymbols(const std::vector<std::wstring>& modulePaths);
    void UnloadModuleSymbols();
};