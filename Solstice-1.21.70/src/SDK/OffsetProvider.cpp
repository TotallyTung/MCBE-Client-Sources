//
// Created by vastrakai on 6/25/2024.
//

#include "OffsetProvider.hpp"

#include <Solstice.hpp>
#include <Utils/MemUtils.hpp>
#include <libhat.hpp>

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()

hat::scan_result OffsetProvider::scanSig(hat::signature_view sig, const std::string& name, int offset)
{
    mSigScanCount++;
    auto minecraft = hat::process::get_process_module();
    auto result = hat::find_pattern(sig, ".text", minecraft);

    if (!result.has_result()) {
        mSigs[name] = 0;
        return {};
    }

    mSigs[name] = reinterpret_cast<uintptr_t>(result.get()) + offset;
    return result;
}

void OffsetProvider::initialize()
{
    int64_t start = NOW;
    #pragma omp parallel for
    for (int i = 0; i < mSigInitializers.size(); i++)
    {
        mSigInitializers[i]();
    }
    uint64_t end = NOW;

    for (const auto& sig : mSigs)
    {
        if (sig.second != 0)
        {
            Solstice::console->info("[offsets] found {} @ {}", sig.first, MemUtils::getMbMemoryString(sig.second));
        }
    }

    for (const auto& sig : mSigs)
    {
        if (sig.second == 0)
        {
            Solstice::console->critical("[offsets] failed to find {}", sig.first);
        }
    }

#ifndef __DEBUG__
    auto mc = hat::process::get_process_module().address();
    auto mcSize = hat::process::get_process_module().get_module_data().size();

    // Terminate if we failed to find a signature
    for (const auto& sig : mSigs) {
        // Make a crash log if the sig is out of bounds
        if (sig.second < mc || sig.second > mc + mcSize) {
            ExceptionHandler::makeCrashLog("An error occurred while initializing: Signature out of bounds!", 0xFF00);
            __fastfail(0);
            std::exit(0);
        }

        if (sig.second == 0) {
            ExceptionHandler::makeCrashLog("An error occurred while initializing!", 0xFF02);
            __fastfail(0);
            std::exit(0);
        }

    }
#endif

    Solstice::console->info("[offsets] initialized in {}ms, {} total sigs scanned", end - start, mSigScanCount);
    mIsInitialized = true;
}

void OffsetProvider::deinitialize()
{
    spdlog::info("[offsets] deinitializing...");

    mSigInitializers.clear();
    mSigs.clear();

    mIsInitialized = false;
}

