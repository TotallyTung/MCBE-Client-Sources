//
// Created by vastrakai on 9/26/2024.
//

#include "MemoryLib.hpp"

// readBytes
class Mem
{
public:
    static std::vector<uint8_t> memReadBytes(hat::scan_result r, size_t size)
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(r.get());

        std::vector<uint8_t> buffer(size);
        memcpy(buffer.data(), reinterpret_cast<void*>(address), size);
        return buffer;
        spdlog::info("[lua] Read {} bytes from {:X}", size, address);
    }

    static void memNopBytes(hat::scan_result r, size_t size)
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(r.get());
        DWORD oldProtect;
        VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memset((void*)address, 0x90, size);
        VirtualProtect((void*)address, size, oldProtect, &oldProtect);
        spdlog::info("[lua] Nopped {} bytes at {:X}", size, address);
    }

    static hat::scan_result memFindPattern(std::string pattern)
    {
        spdlog::info("[lua] Searching for pattern: {}", pattern);
        auto sig = hat::parse_signature(pattern);
        if (!sig.has_value())
        {
            spdlog::error("[lua] Invalid pattern: {}", pattern);
            return hat::scan_result{};
        }


        hat::scan_result result = hat::find_pattern(sig.value(), ".text");
        if (!result.has_result())
        {
            spdlog::error("[lua] Pattern not found: {}", pattern);
            return hat::scan_result{};
        }

        spdlog::info("[lua] Found pattern: {:X}", reinterpret_cast<uintptr_t>(result.get()));

        return result;
    }

    static void memWriteBytes(uintptr_t address, std::vector<uint8_t> bytes)
    {
        DWORD oldProtect;
        VirtualProtect((void*)address, bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((void*)address, bytes.data(), bytes.size());
        VirtualProtect((void*)address, bytes.size(), oldProtect, &oldProtect);
        spdlog::info("[lua] Wrote {} bytes to {:X}", bytes.size(), address);
    }

    // just for lua lol
    static bool equals(void* a, void* b) {
        return a == b;
    }

};

class BytePatchUtil
{
public:
    hat::scan_result result;
    std::vector<uint8_t> originalBytes;
    int size;

    BytePatchUtil(hat::scan_result r, int size)
    {
        result = r;
        originalBytes = Mem::memReadBytes(r, size);
        this->size = size;
    }

    void patch(std::vector<uint8_t> bytes)
    {
        Mem::memWriteBytes(reinterpret_cast<uintptr_t>(result.get()), bytes);
    }

    void nop()
    {
        Mem::memNopBytes(result, size);
    }

    void nop2(int unused)
    {
        Mem::memNopBytes(result, size);
    }

    void restore()
    {
        Mem::memWriteBytes(reinterpret_cast<uintptr_t>(result.get()), originalBytes);
    }

    void ret()
    {
        std::vector<uint8_t> retBytes(size, 0x90);
        retBytes[0] = 0xC3;
        Mem::memWriteBytes(reinterpret_cast<uintptr_t>(result.get()), retBytes);
    }
};

using ByteVector = std::vector<uint8_t>;

void MemoryLib::initialize(lua_State* L)
{
    getGlobalNamespace(L)
        .beginClass<Mem>("Mem")
            .addStaticFunction("findPattern", &Mem::memFindPattern)
            .addStaticFunction("read", &Mem::memReadBytes)
            .addStaticFunction("nop", &Mem::memNopBytes)
            .addStaticFunction("write", &Mem::memWriteBytes)
            .addStaticFunction("equals", [](Actor* a, Actor* b) -> bool { return a == b; })
        .endClass()
    .beginClass<hat::scan_result>("ScanResult")
        .addFunction("get", [](hat::scan_result& result) -> uintptr_t { return reinterpret_cast<uintptr_t>(result.get()); })
        .addFunction("rel", [](hat::scan_result& result, uintptr_t base) -> uintptr_t { return reinterpret_cast<uintptr_t>(result.rel(base)); })
        .addFunction("hasResult", &hat::scan_result::has_result)
    .endClass()
    .beginClass<BytePatchUtil>("BytePatchUtil")
        .addConstructor<void(*)(hat::scan_result, int)>()
        .addFunction("patch", &BytePatchUtil::patch)
        .addFunction("nop", &BytePatchUtil::nop)
        .addFunction("restore", &BytePatchUtil::restore)
        .addFunction("ret", &BytePatchUtil::ret)
    .endClass();

}
