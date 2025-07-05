//
// Created by vastrakai on 9/17/2023.
//

#include "ExceptionHandler.h"

#include <string>
#include <windows.h>
#include <ImageHlp.h>
#include <Psapi.h>
#include <Prax/Utils/Misc.h>
#include <Prax/Utils/String.h>

#include "Config/ConfigManager.h"
#include "Utils/System/Internet.h"



LONG WINAPI TopLevelExceptionHandler(const PEXCEPTION_POINTERS pExceptionInfo)
{
    if (!ExceptionHandler::Handle) return EXCEPTION_CONTINUE_SEARCH;
#ifdef __DEBUG__
    // open a message box and print all the exception info and use String::ToHex to print the exception address


    const std::string exceptionCode = "Exception Code: " + String::ToHex(pExceptionInfo->ExceptionRecord->ExceptionCode) + "\n";
    const std::string exceptionAddress = "Exception Address: " + String::ToHex(reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress)) + "\n";
    const std::string exceptionFlags = "Exception Flags: " + String::ToHex(pExceptionInfo->ExceptionRecord->ExceptionFlags) + "\n";
    const std::string exceptionParams = "Exception Parameters: " + String::ToHex(pExceptionInfo->ExceptionRecord->NumberParameters) + "\n";
    const std::string exceptionContextRecord = "Exception Context Record: " + String::ToHex(reinterpret_cast<uintptr_t>(pExceptionInfo->ContextRecord)) + "\n";
    Misc::SetClipboard(Misc::GetModuleOfAddress(reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress)));

    const std::string fullException = "EXECEPTION" + exceptionCode + exceptionAddress + exceptionFlags + exceptionParams + exceptionContextRecord;

    Logger::Write("ExceptionHandler", fullException, Logger::LogType::Error);

    auto res = MessageBoxA(nullptr, (exceptionCode + exceptionAddress + exceptionFlags + exceptionParams + exceptionContextRecord + "\n\n" + "Base offset (Copied to clipboard): " +
                    Misc::GetModuleOfAddress(reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress))).c_str(), "Catgirl has crashed!", MB_RETRYCANCEL | MB_ICONERROR);

    if (res == IDRETRY) {
        return EXCEPTION_CONTINUE_EXECUTION; // Continue execution and attempt to recover
    }
#endif



    // Get the current time and date
    SYSTEMTIME time;
    GetLocalTime(&time);

    // Convert the time and date to a string
    const std::string timeStr = std::to_string(time.wYear) + "_" + std::to_string(time.wMonth) + "_" + std::to_string(time.wDay) + " " + std::to_string(time.wHour) + "_" + std::to_string(time.wMinute) + "_" + std::to_string(time.wSecond);
    ConfigManager::Save(timeStr);

    MessageBoxA(nullptr, std::string("Catgirl has crashed! We have saved your config as " + timeStr + ".json").c_str(), "Catgirl has crashed!", MB_OK | MB_ICONERROR);



    return EXCEPTION_EXECUTE_HANDLER;
}

void ExceptionHandler::Init() {
#ifdef __DEBUG__
    Logger::Write("ExceptionHandler", "Initialized");
#endif

    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
}
