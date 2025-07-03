//
// Created by vastrakai on 6/25/2024.
//

#include "MemUtils.hpp"
#include <libhat/Access.hpp>
#include <windows.h>
#include <Psapi.h>
#include <libhat/Process.hpp>

#include <spdlog/spdlog.h>



template <typename Ret, typename ... Args>
auto MemUtils::getFunc(void* func)
{
    return reinterpret_cast<Ret(__cdecl*)(Args...)>(func);
}

template <typename Ret, typename ... Args>
auto MemUtils::getVirtualFunc(void* _this, int index)
{
    auto vtable = *reinterpret_cast<uintptr_t**>(_this);
    return reinterpret_cast<Ret(__thiscall*)(void*, Args...)>(vtable[index]);
}

uintptr_t MemUtils::getAddressByIndex(uintptr_t _this, int index)
{
    const auto vtable = *reinterpret_cast<uintptr_t**>(_this);
    return vtable[index];
}

const std::string MemUtils::getMbMemoryString(uintptr_t addr)
{
    MEMORY_BASIC_INFORMATION mbi;
    if(VirtualQuery(reinterpret_cast<void*>(addr), &mbi, sizeof(mbi)))
    {
        char moduleName[MAX_PATH];
        if (GetModuleFileNameA(reinterpret_cast<HMODULE>(mbi.AllocationBase), moduleName, MAX_PATH)) {
            // use fmt or something here
            std::string addrHex = fmt::format("{:X}", addr - reinterpret_cast<uintptr_t>(mbi.AllocationBase));
            std::string result = std::string(moduleName) + "+" + addrHex;
            return result.substr(result.find_last_of('\\') + 1);
        }
    }

    return "unknown";
}

uintptr_t MemUtils::GetVTableFunction(void *_this, int index) {
    uintptr_t *vtable = *reinterpret_cast<uintptr_t **>(_this);
    return vtable[index];
}

std::string MemUtils::getModulePath(HMODULE handle)
{
    char path[MAX_PATH];
    if (GetModuleFileNameExA(GetCurrentProcess(), handle, path, MAX_PATH))
    {
        return path;
    }
    return "";
}

void MemUtils::writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes, size_t size)
{
    DWORD oldProtect;
    VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(reinterpret_cast<void*>(ptr), bytes.data(), size);
    VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
}

void MemUtils::writeBytes(uintptr_t ptr, const void* bytes, size_t size)
{
    DWORD oldProtect;
    VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(reinterpret_cast<void*>(ptr), bytes, size);
    VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
}

void MemUtils::writeBytes(uintptr_t ptr, const std::vector<unsigned char>& bytes)
{
    writeBytes(ptr, bytes, bytes.size());
}

void MemUtils::copyBytes(uintptr_t dest, uintptr_t src, size_t size)
{
    if (dest == 0 || src == 0) {
        spdlog::error("Failed to copy bytes from 0x{:X} to 0x{:X} [Invalid address]", src, dest);
        return;
    }
    DWORD oldprotect;
    VirtualProtect(reinterpret_cast<void*>(dest), size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy(reinterpret_cast<void*>(dest), reinterpret_cast<void*>(src), size);
    VirtualProtect(reinterpret_cast<void*>(dest), size, oldprotect, &oldprotect);
}

std::vector<unsigned char> MemUtils::readBytes(uintptr_t ptr, size_t size)
{
    std::vector<unsigned char> buffer(size);
    DWORD oldProtect;
    VirtualProtect(reinterpret_cast<void*>(ptr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(buffer.data(), reinterpret_cast<void*>(ptr), size);
    VirtualProtect(reinterpret_cast<void*>(ptr), size, oldProtect, &oldProtect);
    return buffer;
}

void MemUtils::ReadBytes(void* address, void* buffer, size_t size) {
    DWORD oldProtect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(buffer, address, size);
    VirtualProtect(address, size, oldProtect, &oldProtect);
}

void MemUtils::NopBytes(uintptr_t address, size_t size) {
    if ((LPVOID)address == nullptr)
        return;

    DWORD oldprotect;
    VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memset((LPVOID)address, 0x90, size);
    VirtualProtect((LPVOID)address, size, oldprotect, &oldprotect);
}

int32_t MemUtils::GetRelativeAddress(uintptr_t address, uintptr_t target) {
    return (uint32_t)((uintptr_t)target - (uintptr_t)address - 4);
}

void MemUtils::setProtection(uintptr_t ptr, size_t size, DWORD protection)
{
    DWORD oldProtect;
    VirtualProtect(reinterpret_cast<void*>(ptr), size, protection, &oldProtect);
}


std::string MemUtils::bytesToHex(char* bytes, int length)
{
    // Convert the bytes to a hex string
    // format: "00 00 00 00"
    std::stringstream ss;

    for (int i = 0; i < length; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (0xFF & bytes[i]);
        if (i + 1 < length)
        {
            ss << " ";
        }
    }

    return ss.str();
}

#define INRANGE(x,a,b)	(x >= a && x <= b)
#define getBits( x )	(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )	(getBits(x[0]) << 4 | getBits(x[1]))

/*uintptr_t Mem::FindSig(uintptr_t rangeStart, uintptr_t rangeEnd, const char* pattern)
{
	const char* pat = pattern;
	DWORD_PTR firstMatch = 0;
	for (DWORD_PTR pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!*pat) return firstMatch;
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;


			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;
			else
				pat += 2;
		}
		else {

			if (firstMatch != 0)
				pCur = firstMatch;
			pat = pattern;
			firstMatch = 0;
		}
	}
	return NULL;
}*/

std::vector<uintptr_t> MemUtils::findPattern(const std::string& pattern)
{
    std::vector<uintptr_t> results;
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));
    int THREAD_COUNT = std::thread::hardware_concurrency();
    uintptr_t start = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
    uintptr_t end = start + moduleInfo.SizeOfImage;
    const char* pat = pattern.c_str();

    for (uintptr_t pCur = start; pCur < end; pCur++)
    {
        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
        {
            if (!*pat) results.push_back(pCur);
            if (!pat[2]) results.push_back(pCur);

            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
                pat += 3;
            else
                pat += 2;
        }
        else
        {
            pat = pattern.c_str();
        }
    }

    return results;

}

uintptr_t MemUtils::findStringInRange(const std::string& string, uintptr_t start, uintptr_t end) {
    const size_t length = string.size();
    const char* target = string.c_str();

    for (uintptr_t pCur = start; pCur < end; pCur++) {
        if (std::memcmp(reinterpret_cast<const char*>(pCur), target, length) == 0) {
            return pCur;
        }
    }
    return 0;
}

uintptr_t MemUtils::findReferenceInRange(uintptr_t ptr, uintptr_t start, uintptr_t end) {
    for (uintptr_t addr = start; addr < end; addr++) {
        uint8_t* bytePtr = reinterpret_cast<uint8_t*>(addr);

        if (bytePtr[0] == 0xE8) {
            int32_t offset = *reinterpret_cast<int32_t*>(addr + 1);
            uintptr_t targetAddr = addr + offset + 5;

            if (targetAddr == ptr) {
                return addr;
            }
        }

        if (bytePtr[0] == 0x48 && bytePtr[1] == 0x8D) {
            int32_t offset = *reinterpret_cast<int32_t*>(addr + 3);
            uintptr_t targetAddr = addr + offset + 7;

            if (targetAddr == ptr) {
                return addr;
            }
        }
    }
    return 0;
}

uintptr_t MemUtils::findString(const std::string& string) {
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));
    uintptr_t start = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
    uintptr_t end = start + moduleInfo.SizeOfImage;

    // Determine number of threads to use
    const size_t numThreads = std::thread::hardware_concurrency();
    const uintptr_t rangeSize = (end - start) / numThreads;

    std::vector<std::future<uintptr_t>> futures;
    for (size_t i = 0; i < numThreads; ++i) {
        uintptr_t rangeStart = start + i * rangeSize;
        uintptr_t rangeEnd = (i == numThreads - 1) ? end : rangeStart + rangeSize;

        futures.emplace_back(std::async(std::launch::async, &MemUtils::findStringInRange, string, rangeStart, rangeEnd));
    }

    for (auto& future : futures) {
        uintptr_t result = future.get();
        if (result != 0) {
            return result;
        }
    }

    return 0;
}

uintptr_t MemUtils::findReference(uintptr_t address) {
    if (address == 0) {
        spdlog::error("Invalid address passed to findReference");
        return 0;
    }
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));
    uintptr_t start = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
    uintptr_t end = start + moduleInfo.SizeOfImage;

    // Determine number of threads to use
    const size_t numThreads = std::thread::hardware_concurrency();
    const uintptr_t rangeSize = (end - start) / numThreads;

    std::vector<std::future<uintptr_t>> futures;
    for (size_t i = 0; i < numThreads; ++i) {
        uintptr_t rangeStart = start + i * rangeSize;
        uintptr_t rangeEnd = (i == numThreads - 1) ? end : rangeStart + rangeSize;

        futures.emplace_back(std::async(std::launch::async, &MemUtils::findReferenceInRange, address, rangeStart, rangeEnd));
    }

    for (auto& future : futures) {
        uintptr_t result = future.get();
        if (result != 0) {
            return result;
        }
    }

    return 0;
}

std::vector<uintptr_t> MemUtils::findReferences(uintptr_t address) {
    std::vector<uintptr_t> results;
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));
    uintptr_t start = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
    uintptr_t end = start + moduleInfo.SizeOfImage;

    for (uintptr_t addr = start; addr < end; addr++) {
        uint8_t* bytePtr = reinterpret_cast<uint8_t*>(addr);

        if (bytePtr[0] == 0xE8) {
            int32_t offset = *reinterpret_cast<int32_t*>(addr + 1);
            uintptr_t targetAddr = addr + offset + 5;

            if (targetAddr == address) {
                results.push_back(addr);
            }
        }

        if (bytePtr[0] == 0x48 && bytePtr[1] == 0x8D) {
            int32_t offset = *reinterpret_cast<int32_t*>(addr + 3);
            uintptr_t targetAddr = addr + offset + 7;

            if (targetAddr == address) {
                results.push_back(addr);
            }
        }
    }

    return results;
}

uintptr_t MemUtils::getTopOfFunction(uintptr_t address)
{
    if (address == 0)
    {
        spdlog::error("Invalid address passed to getTopOfFunction");
        return 0;
    }
    uintptr_t current = address;
    while (true)
    {
        // If we hit a padding 0xCC, or we hit a 0xC3 (ret) instruction, we've reached the top of the function
        if (*reinterpret_cast<uint8_t*>(current) == 0xCC || *reinterpret_cast<uint8_t*>(current) == 0xC3)
        {
            // if current + 1 isn't a multiple of 16, we're not at the top of the function
            if ((current + 1) % 16 == 0)
            {
                return current + 1;
            }
        }

        current--;
    }
}

bool MemUtils::isValidPtr(uintptr_t address)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<void*>(address), &mbi, sizeof(mbi)))
    {
        return mbi.State == MEM_COMMIT;
    }
    return false;
}
