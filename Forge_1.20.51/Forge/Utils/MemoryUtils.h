#pragma once
#include <functional>

template <typename F>
auto IIFE(F f) { return f(); }

template <class T>
struct remove_cvref {
	typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename Ret, typename Type>
Ret& direct_access(Type* type, size_t offset) {
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

#define FAKE_FIELD(type, name)       \
	AS_FIELD(type, name, get##name); \
	type get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                           \
	AS_FIELD(type, name, get##name);                                    \
	type get##name() const { return direct_access<type>(ptr, offset); } \
	void set##name(type v) const { direct_access<type>(ptr, offset) = v; }

#define CLASS_FIELD(type, name, offset)                                  \
	AS_FIELD(type, name, get##name);                                     \
	type get##name() const { return direct_access<type>(this, offset); } \
	void set##name(type v) const { direct_access<type>(this, offset) = v; }

// util used to call functions by their void* address
template <typename R, typename... Args>
R CallFunc(void* func, Args... args) {
	return ((R(*)(Args...))func)(args...);
}

// util used to call functions by their uintptr_t address
template <typename R, typename... Args>
R CallFunc(uintptr_t func, Args... args) {
	return ((R(*)(Args...))func)(args...);
}

template <unsigned int IIdx, typename TRet, typename... TArgs>
static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
	using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
	return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
}

// minecraft base address
uintptr_t range_start = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");

// function used to hook game functions
bool HookFunction(void* pTarget, void* pDetour, void* pOriginal, const char* sigId = "Unknown") {

	static bool initialized = false;

	if (!initialized) {
		initialized = true;
		auto initResult = MH_Initialize();
		if (initResult != 1 && initResult != MH_OK) {
			logF("Failed to initialize MinHook\n");
			return false;
		}
	}

	if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK) {
		logF("[%s, %s] Failed to create hook\n", ssstr(pTarget), sigId);
		return false;
	}

	if (MH_EnableHook(pTarget) != MH_OK) {
		logF("[%s, %s] Failed to enable hook\n", ssstr(pTarget), sigId);
		return false;
	}

	std::stringstream ss;
	ss << std::hex << (uintptr_t)pTarget - range_start;
	logF("%s[  %sOK  %s] Successfully Enabled Hook: %s%s@%s\n", GRAY, GREEN, GRAY, WHITE, sigId, ss.str().c_str());

	return true;

}

static void* findMultiLvlPtr(uintptr_t baseAddr, std::vector<unsigned int> offsets) {

	uintptr_t addr = baseAddr;

	for (int I = 0; I < offsets.size(); I++)
	{
		addr = *(uintptr_t*)(addr);

		if ((uintptr_t*)(addr) == nullptr)
		{
			return reinterpret_cast<void*>(addr);
		}

		addr += offsets[I];
	};

	return reinterpret_cast<void*>(addr);

};

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))

static uintptr_t _findSignature(const char* sig, std::string sigName = "SECRET SIGNATURE") {
	const char* pattern = sig;
	uintptr_t first_match = 0;
	static MODULEINFO mi_mod_info;
	static bool init = false;
	if (!init)
	{
		init = true;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &mi_mod_info, sizeof(MODULEINFO));
	}
	static const uintptr_t range_end = range_start + mi_mod_info.SizeOfImage;

	BYTE pat_byte = GET_BYTE(pattern);
	const char* old_pat = pattern;

	for (uintptr_t p_cur = range_start; p_cur < range_end; p_cur++) // eevrything we use is over 0x100000 so we can just skip that part of memory
	{
		if (!*pattern)
		{
			return first_match;
		}

		while (*(PBYTE)pattern == ' ')
		{
			pattern++;
		}

		if (!*pattern)
		{
			return first_match;
		}

		if (old_pat != pattern)
		{
			old_pat = pattern;
			if (*(PBYTE)pattern != '\?') {
				pat_byte = GET_BYTE(pattern);
			}
		}

		if (*(PBYTE)pattern == '\?' || *(BYTE*)p_cur == pat_byte)
		{
			if (!first_match)
			{
				first_match = p_cur;
			}

			if (!pattern[2])
			{
				return first_match;
			}

			pattern += 2;
		}
		else
		{
			pattern = sig;
			first_match = 0;
		}
	}

	return 0;

}

namespace Game {
	extern bool Initializing;
	namespace RenderInfo {
		extern std::string bootLogLOL;
	}
}

template <typename T> // not related to memory LOL
static std::string combine(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

template <typename T, typename... Args> // not related to memory LOL
static std::string combine(T t, Args... args)
{
	std::stringstream ss;
	ss << t << combine(args...);
	return ss.str();
}

static uintptr_t findSig(const char* sig, const char* offsetId = "SECRET SIGNATURE")
{
	if (Game::Initializing) {
		karnelLogF("sig[1]: Finding Signature: %s....\n", offsetId);
	}
	void* result = (void*)_findSignature(sig);
	if (Game::Initializing) {
		if (result != nullptr)
		{
			std::stringstream ss;
			ss << "Minecraft.Windows.exe+" << std::hex << (uintptr_t)result - range_start;
			Game::RenderInfo::bootLogLOL += Utils::format("[  OK  ] Successfully Found Sig: %s\n", offsetId);
		}
		else
		{
			Game::RenderInfo::bootLogLOL += Utils::format("[FAILED] Failed Finding Sig: %s", offsetId);
		}
	}

	return (uintptr_t)result;
}