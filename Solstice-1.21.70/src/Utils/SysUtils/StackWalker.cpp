//
// Created by vastrakai on 7/7/2024.
//

#include "StackWalker.hpp"
#define _AMD64_
#include <DbgHelp.h>
#include <spdlog/spdlog.h>
#include <Utils/MemUtils.hpp>
#include <Utils/ProcUtils.hpp>

StackWalker::StackWalker()
{
    // Get the module paths
    std::vector<std::wstring> modulePaths = ProcUtils::getModulePaths();

    // Load the symbols for the modules
    LoadModuleSymbols(modulePaths);

    // Initialize the symbol handler
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    if (!SymInitialize(GetCurrentProcess(), nullptr, TRUE))
    {
        spdlog::error("Failed to initialize symbol handler");
    }
}

StackWalker::~StackWalker()
{
    // Cleanup the symbol handler
    UnloadModuleSymbols();
}

// Stored vector of loaded module paths
static std::vector<std::wstring> gModulePaths;

void StackWalker::LoadModuleSymbols(const std::vector<std::wstring>& modulePaths) {
    HMODULE mainModule = Solstice::mModule;
    std::string modulePathStr = MemUtils::getModulePath(mainModule);
    if (!modulePathStr.empty()) {
        gModulePaths.emplace_back(modulePathStr.begin(), modulePathStr.end());

        DWORD symOptions = SymGetOptions();
        symOptions |= SYMOPT_LOAD_LINES;
        symOptions |= SYMOPT_UNDNAME;
        SymSetOptions(symOptions);

        DWORD64 result = SymLoadModuleEx(GetCurrentProcess(), Solstice::mModule, modulePathStr.c_str(), nullptr, 0, 0,
                                         nullptr, 0);

        if (result == 0) {
            spdlog::error("Failed to load symbols for the main module.");
        } else {
            spdlog::info("Loaded symbols for Solstice.dll.");
        }




    } else {
        spdlog::error("Could not get module path for the main module.");
    }
}

void StackWalker::UnloadModuleSymbols()
{
    SymUnloadModule64(GetCurrentProcess(), SymGetModuleBase64(GetCurrentProcess(), 0));
    SymCleanup(GetCurrentProcess());
}


std::vector<std::string> StackWalker::ShowCallstack(HANDLE hThread, PCONTEXT pContext) {
    auto stackTrace = std::vector<std::string>();
    // Initialize the stack frame
    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));
    stackFrame.AddrPC.Offset = pContext->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = pContext->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = pContext->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

    // Initialize the context record
    CONTEXT contextRecord;
    memset(&contextRecord, 0, sizeof(CONTEXT));
    contextRecord.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&contextRecord);

    // Initialize the symbol info
    SYMBOL_INFO* symbolInfo = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR));
    memset(symbolInfo, 0, sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR));
    symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbolInfo->MaxNameLen = MAX_SYM_NAME;

    // Initialize the line info
    IMAGEHLP_LINE64 lineInfo;
    memset(&lineInfo, 0, sizeof(IMAGEHLP_LINE64));
    lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    // Walk the stack
    while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), hThread, &stackFrame, &contextRecord, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
    {
        if (SymFromAddr(GetCurrentProcess(), stackFrame.AddrPC.Offset, nullptr, symbolInfo))
        {
            if (SymGetLineFromAddr64(GetCurrentProcess(), stackFrame.AddrPC.Offset, nullptr, &lineInfo))
            {
                spdlog::info("{} {} - {}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset), symbolInfo->Name, lineInfo.LineNumber);
                stackTrace.push_back(fmt::format("{} {} - {}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset), symbolInfo->Name, lineInfo.LineNumber));
            }
            else
            {
                spdlog::info("{} {}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset), symbolInfo->Name);
                stackTrace.push_back(fmt::format("{} {}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset), symbolInfo->Name));
            }
        }
        else
        {
            // If we can't get the symbol name, just show the raw address
            spdlog::info("{}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset));
            stackTrace.push_back(fmt::format("{}", MemUtils::getMbMemoryString(stackFrame.AddrPC.Offset)));
        }
    }

    // Cleanup
    free(symbolInfo);

    return stackTrace;
}
