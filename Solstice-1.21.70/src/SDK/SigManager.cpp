//
// Created by vastrakai on 6/24/2024.
//

#include "SigManager.hpp"

#include <iostream>
#include <Solstice.hpp>
#include <Utils/Logger.hpp>
#include <Utils/MemUtils.hpp>
#include <chrono>
#include <omp.h>
#include <libhat.hpp>

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()

hat::scan_result SigManager::scanSig(hat::signature_view sig, const std::string& name, int offset)
{
    mSigScanCount++;

    auto minecraft = hat::process::get_process_module();
    auto result = hat::find_pattern(sig, ".text", minecraft);

    if (!result.has_result()) {
        mSigs[name] = 0;
        return {};
    }

    if (offset == 0) mSigs[name] = reinterpret_cast<uintptr_t>(result.get());
    else mSigs[name] = reinterpret_cast<uintptr_t>(result.rel(offset));

    return result;
}

void SigManager::initialize()
{
    int64_t start = NOW;
    #pragma omp parallel for
    for (int i = 0; i < mSigInitializers.size(); i++) {
        mSigInitializers[i]();
    }
    int64_t end = NOW;
    int64_t diff = end - start;

    for (const auto& sig : mSigs) {
        if (sig.second != 0) Solstice::console->info("[signatures] found {} @ {}", sig.first, MemUtils::getMbMemoryString(sig.second));
    }

    for (const auto& sig : mSigs) {
        if (sig.second == 0) Solstice::console->critical("[signatures] failed to find {}", sig.first);

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

    Solstice::console->info("[signatures] initialized in {}ms, {} total sigs scanned", diff, mSigScanCount);
    mIsInitialized = true;
}

void SigManager::deinitialize()
{
    spdlog::info("[signatures] deinitializing...");
    mSigInitializers.clear();
    mSigs.clear();

    mIsInitialized = false;
}
