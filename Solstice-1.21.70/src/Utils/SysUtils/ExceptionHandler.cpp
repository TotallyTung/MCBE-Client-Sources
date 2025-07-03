//
// Created by vastrakai on 7/7/2024.
//

#include "ExceptionHandler.hpp"

#include <build_info.h>
#include <Windows.h>
#include <string>
#include <spdlog/spdlog.h>

#include "StackWalker.hpp"

LONG WINAPI TopLevelExceptionHandler(const PEXCEPTION_POINTERS pExceptionInfo)
{
    // Get the exception code
    auto exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;

    std::string text = "An unhandled exception occurred. (0x" + fmt::format("{:X}", exceptionCode) + ")\nStack Trace:\n";

    // Get the stack trace using stackwalker
    StackWalker sw;
    std::vector<std::string> stackTrace = sw.ShowCallstack(GetCurrentThread(), pExceptionInfo->ContextRecord);
    for (const auto& line : stackTrace)
    {
        text += line + "\n";
    }
    spdlog::error(text);

    ExceptionHandler::makeCrashLog(text, exceptionCode);

    auto result = MessageBoxA(nullptr, LPCSTR(text.c_str()), "Unhandled Exception", MB_ABORTRETRYIGNORE | MB_ICONERROR);

    // deconstruct stack trace
    stackTrace.clear();
    sw.~StackWalker();


    switch (result)
    {
    case IDABORT:   // Terminates the process
        exit(1);
    case IDRETRY:
        return EXCEPTION_CONTINUE_EXECUTION;
    case IDIGNORE:
        return EXCEPTION_CONTINUE_SEARCH;
    default:
        return EXCEPTION_EXECUTE_HANDLER;
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

void ExceptionHandler::init()
{
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
}

void ExceptionHandler::makeCrashLog(const std::string& text, DWORD exceptionCode)
{
    std::string excPath = FileUtils::getSolsticeDir() + xorstr_("crash.log");

    std::ofstream excFile(excPath, std::ios::app);

    if (excFile.is_open())
    {
        // Prepend the date and time to the crash log
        auto now = std::chrono::system_clock::now();
        auto nowTime = std::chrono::system_clock::to_time_t(now);
        excFile << xorstr_("----------------- Crash at ") << std::put_time(std::localtime(&nowTime), xorstr_("%Y-%m-%d %X")) << xorstr_("\n");
        excFile << text << xorstr_("\n\n");


        auto modules = gFeatureManager ? gFeatureManager->mModuleManager->getModules() : std::vector<std::shared_ptr<Module>>();
        for (const auto& module : modules)
        {
            // Append modules and the mEnabled state
            excFile << module->mName << xorstr_(" - ") << (module->mEnabled ? xorstr_("Enabled") : xorstr_("Disabled")) << xorstr_("\n");
        }
        if (gFeatureManager)
        {
            // Append the module count
            excFile << xorstr_("Module count: ") << modules.size() << xorstr_("\n");
        } else
        {
            excFile << xorstr_("(FeatureManager not initialized)\n");
        }

        // Append the exception code then divider
        excFile << xorstr_("Exception Code: 0x") << fmt::format(xorstr_("{:X}"), exceptionCode) << xorstr_("\n");
        excFile << xorstr_("Solstice commit: ") << SOLSTICE_BUILD_VERSION << xorstr_("\n");
        excFile << xorstr_("Solstice branch: ") << SOLSTICE_BUILD_BRANCH << xorstr_("\n");
        excFile << xorstr_("Solstice commit msg: ") << SOLSTICE_BUILD_COMMIT_MESSAGE << xorstr_("\n");
        excFile << xorstr_("Minecraft version: ") << ProcUtils::getVersion() << xorstr_("\n");
        excFile << xorstr_("----------------------------------------\n");
        excFile.flush();
        excFile.close();
    }
}
