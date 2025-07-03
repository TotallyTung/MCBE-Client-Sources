#include "MemoryUtil.h"
#include "Logger.h"
#include <libhat/Scanner.hpp>

uintptr_t MemoryUtil::getGameAddress() {
	static uintptr_t address = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
	return address;
}

uintptr_t MemoryUtil::findSignature(std::string_view sig) {
	auto address = hat::parse_signature(sig);

	if (!address.has_value()) {
		logF("Sig dead: %s", sig.data())
		return 0;
	}

	auto result = hat::find_pattern(address.value());

	if (result.has_result())
		return reinterpret_cast<uintptr_t>(result.get());

	logF("Sig dead: %s", sig.data());
	return 0;
}

uintptr_t** MemoryUtil::getVtableFromSig(std::string_view sig) {
	uintptr_t address = findSignature(sig);

	if (address == 0x0) 
		return nullptr;

	int finalOffset = *reinterpret_cast<int*>((address + 3));
	return reinterpret_cast<uintptr_t**>(address + finalOffset + 7);
}