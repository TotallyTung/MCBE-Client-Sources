// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include <memory>
#include <MinHook.h>
#include <Psapi.h>
#include "../SDK/Signatures.h"
#include "Utils.h"

#define findSig(Sig) MemoryUtils::findSigLibhat(Sig)

class MemoryUtils {
public:
	static inline uintptr_t getBase() {
		static const uintptr_t cachedBase = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
		return cachedBase;
	}

	static uintptr_t findSigLibhat(hat::signature_view sig) {
		const hat::scan_result result = hat::find_pattern(sig, ".text", hat::process::get_process_module());
		const uintptr_t address = reinterpret_cast<uintptr_t>(result.get());
		return address;
	}

	static uintptr_t** getVtableFromSignature(hat::signature_view sig, int offset) {
		const uintptr_t sigOffset = findSigLibhat(sig);
		if (sigOffset == 0x0) return nullptr;
		const int finalOffset = *reinterpret_cast<int*>((sigOffset + offset));
		auto** signatureOffset = reinterpret_cast<uintptr_t**>(sigOffset + finalOffset + 7);
		return signatureOffset;
	}
	static inline uintptr_t getFuncFromCall(uintptr_t address) {
		if (address == 0) return 0;
		return address + 1 + 4 + *(int*)(address + 1); // Trong smart code
	}

	template <unsigned int IIdx, typename TRet, typename... TArgs>
	static inline TRet CallVFunc(void* thisptr, TArgs... argList) {
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	static void CreateHook(const char* name, uintptr_t address, void* func, void* OriginFunc) {
		if (!address) {
			logF("[%s] Invalid address!", name); 
			return;
		}
		if (MH_CreateHook((void*)address, func, reinterpret_cast<LPVOID*>(OriginFunc)) != MH_OK) {
			logF("[%s] Failed to create hook!", name);
			return;
		}
		if (MH_EnableHook((void*)address) != MH_OK) {
			logF("[%s] Failed to enable hook!", name);
			return;
		}

		//logF("[%s] Successfully hooked!", name);
	}

	static inline void nopBytes(void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	static inline void copyBytes(void* src, void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(src, size, oldprotect, &oldprotect);
	}

	static inline void patchBytes(void* dst, void* src, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}
};

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

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

//fake class macro to avoid compile erros when using pragma once 

#define FK(typep) \
class typep;


template <typename TreatAs, typename Pointer>
constexpr TreatAs& directAccess(Pointer ptr, ptrdiff_t offset) {
	return *reinterpret_cast<TreatAs*>(reinterpret_cast<uintptr_t>(ptr) + offset);
}

#define CLASS_FIELD(name, offset, ...)                                                               \
	__declspec(property(get = GET_FIELD_##name, put = SET_FIELD_##name)) __VA_ARGS__ name;           \
	__VA_ARGS__& GET_FIELD_##name() const { return directAccess<__VA_ARGS__>(this, offset); } \
	template <typename T>                                                                            \
	void SET_FIELD_##name(const T& value) { directAccess<__VA_ARGS__>(this, offset) = value; }





#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return direct_access<type>(ptr, offset); }												     \
void set##name(type v) const { direct_access<type>(ptr, offset) = v; }