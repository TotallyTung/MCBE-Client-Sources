#pragma once

template <typename Ret, typename Type>
// The type and offset
Ret& DirectAccess(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define CLASS_FIELD(type, name, offset)                                                                             \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return DirectAccess<type>(this, offset); }												 \
void set##name(type v) const { DirectAccess<type>(this, offset) = v; }

#define DEFINE_NOP_PATCH_FUNC(name, addr, size) \
        void name(bool patch) { \
            static std::vector<unsigned char> ogBytes; \
            static bool wasPatched = false; \
            if (addr == 0) { \
                return; \
            } \
            if (patch) { \
                if (!wasPatched) { \
                    ogBytes = Memory::readBytes(addr, size); \
                    std::vector<unsigned char> bytes(size, 0x90); \
                    Memory::writeBytes(addr, bytes); \
                    wasPatched = true; \
                } \
            } else { \
                if (wasPatched) { \
                    Memory::writeBytes(addr, ogBytes); \
                    wasPatched = false; \
                } \
            } \
        }


namespace Memory {
#define log(str) OutputDebugStringA(str)

    template <typename R, typename... Args>
    R CallFunc(void* func, Args... args)
    {
        return ((R(*)(Args...))func)(args...);
    }

    template <typename TRet, typename... TArgs>
    static TRet CallFastcall(void* func, TArgs... args)
    {
        using Fn = TRet(__fastcall*)(TArgs...);
        Fn f = reinterpret_cast<Fn>(func);
        return f(args...);
    }

    template <typename TRet, typename... TArgs>
    static TRet CallFastcall(uintptr_t func, TArgs... args)
    {
        using Fn = TRet(__fastcall*)(TArgs...);
        Fn f = reinterpret_cast<Fn>(func);
        return f(args...);
    }

    template<typename Ret, typename... Args>
    static auto CallVirtualFunc(int index, void* _this, Args... args)
    {
        using Fn = Ret(__thiscall*)(void*, Args...);
        auto vtable = *reinterpret_cast<uintptr_t**>(_this);
        return reinterpret_cast<Fn>(vtable[index])(_this, args...);
    }

    template <int IIdx, typename TRet, typename... TArgs>
    static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    static const uintptr_t ExecutableBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));

    template <typename T>
    bool HookFunction(void* pTarget, T pDetour, void* pOriginal, const char* SignatureIdName)
    {
        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            if (MH_Initialize() != MH_OK)
            {
                Console::critical("[hooks] failed to initialize MinHook for the client.");
                return false;
            }
        }

        if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK)
        {
            Console::critical(std::string("[hooks] failed to create hook for ") + SignatureIdName);
            return false;
        }

        if (MH_EnableHook(pTarget) != MH_OK)
        {
            Console::critical(std::string("[hooks] failed to enable hook for ") + SignatureIdName);
            return false;
        }
        
        Console::debug(std::string("[hooks] applied hook for ") + SignatureIdName);

        return true;
    }

    static unsigned __int64 resolveRef(unsigned __int64 addr) 
    {
        unsigned char* bytes = (unsigned char*)addr;
        if (bytes[0] == 0xE8 || bytes[0] == 0xE9) return addr + *reinterpret_cast<int*>(addr + 1) + 5;
        if (bytes[0] == 0x48 && bytes[1] == 0x8D && (bytes[2] == 0x05 || bytes[2] == 0x0D) || bytes[0] == 0x4C && bytes[1] == 0x8D && bytes[2] == 0x25) return addr + *reinterpret_cast<int*>(addr + 3) + 7;

        return addr;
    }

    static void writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes, size_t size) 
    {
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(reinterpret_cast<void*>(ptr), bytes.data(), size);
        VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
    }

    static void writeBytes(uintptr_t ptr, const void* bytes, size_t size) 
    {
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(reinterpret_cast<void*>(ptr), bytes, size);
        VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
    }

    static void writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes) 
    {
        writeBytes(ptr, bytes, bytes.size());
    }

    static std::vector<unsigned char> readBytes(uintptr_t ptr, size_t size) 
    {
        std::vector<unsigned char> buffer(size);
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(buffer.data(), reinterpret_cast<void*>(ptr), size);
        VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
        return buffer;
    }

    static void setProtection(uintptr_t ptr, size_t size, DWORD protection) 
    {
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(ptr), size, protection, &oldProtect);
    }
}