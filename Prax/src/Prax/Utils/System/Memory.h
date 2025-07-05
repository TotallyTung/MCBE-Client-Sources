#pragma once


#include <memory>
#include <string>
#include <entity/registry.hpp>
#include <Prax/SDK/Game/Entity/EntityId.h>


#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))


#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { if (ptr) return direct_access<type>(ptr, offset); else return (type)0; }                                                     \
void set##name(type v) const {if (ptr) direct_access<type>(ptr, offset) = v; }


// Specialization for std::string
#define BUILD_ACCESS_STRING(ptr, name, offset) \
AS_FIELD(std::string, name, get##name);        \
std::string get##name() const { if (ptr) return direct_access<std::string>(ptr, offset); else return ""; } \
void set##name(std::string v) const {if (ptr) direct_access<std::string>(ptr, offset) = v; }

class EntityRegistry;

struct EntityContext {
    EntityRegistry* mRegistry;
    entt::basic_registry<EntityId>* registry;
    EntityId entityId;

    template <typename T>
    T* getComponent() {
        return const_cast<T*>(registry->try_get<T>(entityId));
    }


};

template<typename TRet, typename... TArgs>
static inline TRet CallFastcall(void* func, TArgs... args) {
    using Fn = TRet(__fastcall*)(TArgs...);
    Fn f = (Fn)func;
    return f(args...);
}

template <typename F> auto IIFE(F f) { return f(); }



template <typename Ret, typename Type> Ret& direct_access(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

class Memory {

public:

    template<int IOff, typename TRet, typename... TArgs>
    static inline TRet CallVFunc(void* vTable, TArgs... args) {
        using Fn = TRet(__fastcall*)(void*, TArgs...);
        Fn* vtable = *static_cast<Fn**>(vTable);
        return vtable[IOff](vTable, args...);
    }

    template <typename TRet, typename... TArgs>
    inline auto* GetFastcall(void* func) {
        using Fn = TRet(__fastcall*)(TArgs...);
        return reinterpret_cast<Fn>(func);
    }

    // CallFastcall using above template
    template <typename TRet, typename... TArgs>
    static inline TRet CallFastcall(void* func, TArgs... args) {
        // Return if function is null
        if (!func) return (TRet)0;
        using Fn = TRet(__fastcall*)(TArgs...);
        Fn f = (Fn)func;
        return f(args...);
    }

    // Templated write
    template<typename T>
    static void Write(uintptr_t address, T value) {
        // oldProtect is a pointer to the old protection
        DWORD oldProtect;
        VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
        *(T*)address = value;
        VirtualProtect((void*)address, sizeof(T), oldProtect, &oldProtect);
    }

    static uintptr_t GetAddressByIndex(uintptr_t vtable, int index);
    static uintptr_t SearchAOB(const std::string& signature);
    static uintptr_t ResolveRef(uintptr_t ptr, int offset = 1);
    static int32_t GetRelativeAddress(uintptr_t ptr, uintptr_t endAddress);
    static void WriteBytes(uintptr_t address, int32_t *bytes, int length, bool);
    static void WriteBytes(uintptr_t address, uint32_t *bytes, int length);
    static void WriteBytes(uintptr_t address, std::string bytes, int length);
    static uintptr_t GetDirectAddress(uintptr_t intAddress);
    static void SetProtection(uintptr_t addr, size_t size, DWORD protect);
    static void ReadBytes(void *address, void *buffer, size_t size);
    static std::vector<uint8_t> ReadBytes(uintptr_t address, int size);
    static void WriteBytes(uintptr_t addr, const std::vector<unsigned char>& bytes, int length);
    static uintptr_t ScanSig(const char* sig, int offset = 0);

};

template<typename T>
struct SharedCounter {
    /* volatile ? */ T* value;
    std::atomic<int> shared, weak;

    SharedCounter(T* value) : value(value) {}

    void addSharedRef() { ++shared; }

    void addWeakRef() { --shared; }

    bool releaseSharedRef() {
        if (--shared == 0) {
            if (value != nullptr) {
                T* oldValue = value;
                value = nullptr;
                delete oldValue;
            }
            return (weak == 0);
        }
        return false;
    }

    bool releaseWeakRef() {
        return (--weak == 0 && value);
    }
};

template<typename T>
struct WeakPtr {
    SharedCounter<T>* counter = nullptr;

    WeakPtr(T* val = nullptr) {
        if (val) {
            counter = new SharedCounter<T>(val);
            counter->addWeakRef();
        }
    }

    WeakPtr(WeakPtr&& ptr) {
        counter = std::move(ptr.counter);
        ptr.counter = nullptr;
    }

    WeakPtr& operator=(WeakPtr const& ptr) {
        reset();
        this->counter = ptr.counter;
        if (counter)
            counter->addWeakRef();
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& ptr) {
        counter = std::move(ptr.counter);
        ptr.counter = nullptr;
        return *this;
    }

    void reset() {
        if (counter && counter->releaseWeakRef())
            delete counter;
        counter = nullptr;
    }

    ~WeakPtr() {
        reset();
    };

    template <typename... Args>
    static WeakPtr<T> make(Args&&... args) {
        return WeakPtr<T>(new T(std::forward(args...)));
    }

    T& operator*() {
        return *counter->value;
    }

    T* operator->() {
        return counter->value;
    }

    T* get() {
        if (!counter)
            return nullptr;
        return counter->value;
    }

    T const* get() const {
        if (!counter)
            return nullptr;
        return counter->value;
    }
};

template<typename T>
struct SharedPtr {
    SharedCounter<T>* counter = nullptr;

    SharedPtr(T* val = nullptr) {
        if (val) {
            counter = new SharedCounter<T>(val);
            counter->addSharedRef();
        }
    }

    SharedPtr(SharedPtr const& a) : counter(a.counter) {
        if (counter)
            counter->addSharedRef();
    }

    void reset() {
        if (counter && counter->releaseSharedRef())
            delete counter;
        counter = nullptr;
    }

    SharedPtr& operator=(SharedPtr const& ptr) {
        reset();
        this->counter = ptr.counter;
        if (counter)
            counter->addSharedRef();
        return *this;
    }

    ~SharedPtr() {
        reset();
    }

    template <typename... Args>
    static SharedPtr<T> make(Args&&... args) {
        return SharedPtr<T>(new T(std::forward(args...)));
    }

    T& operator*() {
        return *counter->value;
    }

    T* operator->() {
        return counter->value;
    }

    T* get() {
        if (!counter)
            return nullptr;
        return counter->value;
    }
};