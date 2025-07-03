#pragma once
#include <cstdint>
#include <string>
#include <atomic>
#define _AMD64_
#include <minwindef.h>
#include <unordered_map>
#include <Features/Events/SendImmediateEvent.hpp>
//
// Created by vastrakai on 6/25/2024.
//


template<typename Func, typename... Args>
bool TryCallWrapper(Func func, Args&&... args) {
    __try
    {
        func(std::forward<Args>(args)...);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return false;
    }
}

// Refactored TRY_CALL using lambdas and spdlog
#define TRY_CALL(func, ...) \
[&]() { \
bool result = TryCallWrapper([&]() { func(__VA_ARGS__); }); \
if (!result) { \
spdlog::error("Exception thrown in {} at line {} in {}", __FUNCTION__, __LINE__, __FILE__); \
} \
return result; \
}()

// lol
namespace serenity::utils::ptr {

    using ptr_t = union ptr_data_t {
        uintptr_t address;
        void *v_ptr;
        uintptr_t hat_module;

        template <typename pointing_t>
        pointing_t as() {
            return reinterpret_cast<pointing_t>(this->v_ptr);
        }

        [[nodiscard]] ptr_data_t offset(const uintptr_t p_offset) const {
            return ptr_data_t { .address = this->address + p_offset };
        }
    };

    inline ptr_t ptr(const uintptr_t p_address) {
        ptr_data_t data {};
        data.address = p_address;
        return data;
    }

    template <typename pointing_t>
    ptr_t ptr(pointing_t p_ptr) {
        return ptr_data_t { .v_ptr = reinterpret_cast<void*>(p_ptr) };
    }
}

using namespace serenity::utils::ptr;


class MemUtils {
public:
    template<typename Ret, typename... Args>
    static auto getFunc(void* func);
    template<typename Ret, typename... Args>
    static auto getVirtualFunc(void* _this, int index);

    template <typename TRet, typename... TArgs>
    static TRet callFastcall(void* func, TArgs... args)
    {
        using Fn = TRet(__fastcall*)(TArgs...);
        Fn f = reinterpret_cast<Fn>(func);
        return f(args...);
    }

    template <typename TRet, typename... TArgs>
    static TRet callFastcall(uintptr_t func, TArgs... args)
    {
        using Fn = TRet(__fastcall*)(TArgs...);
        Fn f = reinterpret_cast<Fn>(func);
        return f(args...);
    }

    template<typename Ret, typename... Args>
    static auto callVirtualFunc(int index, void* _this, Args... args)
    {
        using Fn = Ret(__thiscall*)(void*, Args...);
        auto vtable = *reinterpret_cast<uintptr_t**>(_this);
        return reinterpret_cast<Fn>(vtable[index])(_this, args...);
    }

    template<typename T>
    static void Write(uintptr_t address, T value) {
        // oldProtect is a pointer to the old protection
        DWORD oldProtect;
        VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
        *(T*)address = value;
        VirtualProtect((void*)address, sizeof(T), oldProtect, &oldProtect);
    }

    /// <summary>
    /// Defines a function to byte-patch an address with a given byte array
    /// </summary>
    /// <remarks>
    /// This should only be used in .cpp files
    /// </remarks>
    /// <param name="name">The name of the function</param>
    /// <param name="addr">The address to patch</param>
    /// <param name="bytes">The bytes to replace the address with</param>
    #define DEFINE_PATCH_FUNC(name, addr, bytes) \
        void name(bool patch) { \
            static std::vector<unsigned char> ogBytes = { bytes }; \
            static bool wasPatched = false; \
            if (addr == 0) { \
                spdlog::error("Failed to patch {} at 0x{:X} [Invalid address]", #name, addr); \
                return; \
            } \
            if (patch) { \
                 if (!wasPatched) { \
                     ogBytes = MemUtils::readBytes(addr, bytes.size()); \
                     MemUtils::writeBytes(addr, bytes); \
                     spdlog::trace("Patched {} at 0x{:X}", #name, addr); \
                     wasPatched = true; \
                } \
            } else { \
                if (wasPatched) { \
                    MemUtils::writeBytes(addr, ogBytes); \
                    spdlog::trace("Unpatched {} at 0x{:X}", #name, addr); \
                    wasPatched = false; \
                } \
            } \
        }

    /// <summary>
    /// Defines a function to NOP-patch an address with a given size
    /// </summary>
    /// <remarks>
    /// This should only be used in .cpp files
    /// </remarks>
    /// <param name="name">The name of the function</param>
    /// <param name="addr">The address to patch</param>
    /// <param name="size">The amount of bytes to replace with NOP instructions</param>
    #define DEFINE_NOP_PATCH_FUNC(name, addr, size) \
        void name(bool patch) { \
            static std::vector<unsigned char> ogBytes; \
            static bool wasPatched = false; \
            if (addr == 0) { \
                spdlog::error("Failed to patch {} at 0x{:X} [Invalid address]", #name, addr); \
                return; \
            } \
            if (patch) { \
                if (!wasPatched) { \
                    ogBytes = MemUtils::readBytes(addr, size); \
                    std::vector<unsigned char> bytes(size, 0x90); \
                    MemUtils::writeBytes(addr, bytes); \
                    spdlog::trace("Patched {} at 0x{:X}", #name, addr); \
                    wasPatched = true; \
                } \
            } else { \
                if (wasPatched) { \
                    MemUtils::writeBytes(addr, ogBytes); \
                    spdlog::trace("Unpatched {} at 0x{:X}", #name, addr); \
                    wasPatched = false; \
                } \
            } \
        }


    #define PATCH_BYTES(addr, bytes) MemUtils::writeBytes(addr, bytes)


    #define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name

    #define CLASS_FIELD(type, name, offset) \
        AS_FIELD(type, name, get##name); \
        type get##name() const { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); } \
        void set##name(type v) const { *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset) = std::move(v); }

    #define TOKENPASTE(x, y) x ## y
    #define TOKENPASTE2(x, y) TOKENPASTE(x, y)
    #define PAD(size) \
        private: \
            char TOKENPASTE2(padding_, __LINE__) [size] = {0}; \
        public:
    /// <summary>
    /// Defines a padding field of a given size
    /// </summary>
    /// <param name="size">The size of the padding</param>
    #define PADDING(size) char TOKENPASTE2(padding_, __LINE__) [size]

    static uintptr_t getAddressByIndex(uintptr_t _this, int index);

    static const std::string getMbMemoryString(uintptr_t addr);
    static int32_t GetRelativeAddress(uintptr_t ptr, uintptr_t endAddress);
    static uintptr_t GetVTableFunction(void *_this, int index);
    static std::string getModulePath(HMODULE handle);
    static void writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes, size_t size);
    static void writeBytes(uintptr_t ptr, const void* bytes, size_t size);
    static void writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes);
    static void copyBytes(uintptr_t dest, uintptr_t src, size_t size);
    static std::vector<unsigned char> readBytes(uintptr_t ptr, size_t size);
    static void ReadBytes(void *address, void *buffer, size_t size);
    static void setProtection(uintptr_t ptr, size_t size, DWORD protection);
    static std::string bytesToHex(char* bytes, int length);
    static std::vector<uintptr_t> findPattern(const std::string& pattern);
    static void NopBytes(uintptr_t address, size_t size);
public:
    static uintptr_t findString(const std::string& string);
    static uintptr_t findReference(uintptr_t address);
    static std::vector<uintptr_t> findReferences(uintptr_t address);
    static uintptr_t getTopOfFunction(uintptr_t address);
    static bool isValidPtr(uintptr_t address);

private:
    template<typename T>
    static T readMemory(uintptr_t address);

    static uintptr_t findStringInRange(const std::string& string, uintptr_t start, uintptr_t end);
    static uintptr_t findReferenceInRange(uintptr_t ptr, uintptr_t start, uintptr_t end);
};


