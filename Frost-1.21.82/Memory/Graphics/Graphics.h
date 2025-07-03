#pragma once

class Graphics {
public:
    struct GPU 
    {
        static std::vector<std::string> getGraphicsCardNames() 
        {
            std::vector<std::string> gpuNames;
            IDXGIFactory* pFactory = nullptr;

            if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory)))) {
                gpuNames.push_back("Unable to create DXGI Factory");
                return gpuNames;
            }

            IDXGIAdapter* pAdapter = nullptr;
            for (UINT i = 0; SUCCEEDED(pFactory->EnumAdapters(i, &pAdapter)); ++i) {
                DXGI_ADAPTER_DESC desc;
                if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
                    std::wstring ws(desc.Description);
                    gpuNames.emplace_back(ws.begin(), ws.end());
                }
                pAdapter->Release();
            }

            if (gpuNames.empty()) {
                gpuNames.push_back("No adapters found");
            }

            pFactory->Release();
            return gpuNames;
        }

        static std::string getUsedGraphicsCardName()
        {
            IDXGIFactory* pFactory = nullptr;
            if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
                return "Unable to create DXGI Factory";
            }

            IDXGIAdapter* pAdapter = nullptr;
            if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) {
                pFactory->Release();
                return "No adapter found";
            }

            DXGI_ADAPTER_DESC desc;
            if (FAILED(pAdapter->GetDesc(&desc))) {
                pAdapter->Release();
                pFactory->Release();
                return "Failed to get adapter description";
            }

            std::wstring ws(desc.Description);
            std::string gpuName(ws.begin(), ws.end());

            pAdapter->Release();
            pFactory->Release();
            return gpuName;
        }
    };

    struct CPU
    {
        static std::string getCPUName() 
        {
            std::array<int, 4> cpui;
            std::string cpuName;
            char name[49] = {};

            __cpuid(cpui.data(), 0x80000000);
            if (cpui[0] < 0x80000004) {
                return "CPUID extended functions not supported";
            }

            // Retrieve CPU name
            for (unsigned int i = 0; i < 3; ++i) {
                __cpuid(reinterpret_cast<int*>(cpui.data()), 0x80000002 + i);
                memcpy(name + (i * 16), cpui.data(), sizeof(cpui));
            }

            cpuName = std::string(name);
            return cpuName.empty() ? "Unknown CPU" : cpuName;
        }
    };

    struct Memory
    {
        static std::string getMemoryStats() 
        {
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);

            if (!GlobalMemoryStatusEx(&memInfo)) {
                return "Failed to retrieve memory status";
            }

            DWORDLONG totalMemory = memInfo.ullTotalPhys;
            DWORDLONG freeMemory = memInfo.ullAvailPhys;
            DWORDLONG usedMemory = totalMemory - freeMemory;
            double percentUsed = (double(usedMemory) / totalMemory) * 100;

            // Format output
            std::ostringstream oss;
            oss.precision(2);
            oss << "Used: " << percentUsed << "% ("
                << usedMemory / (1024 * 1024 * 1024) << " GB / "
                << totalMemory / (1024 * 1024 * 1024) << " GB)";

            return oss.str();
        }

        static DWORDLONG getTotalMemory() 
        {
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);

            if (!GlobalMemoryStatusEx(&memInfo)) {
                return 0;
            }

            return memInfo.ullTotalPhys;
        }

        static DWORDLONG getFreeMemory() 
        {
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);

            if (!GlobalMemoryStatusEx(&memInfo)) {
                return 0;
            }

            return memInfo.ullAvailPhys;
        }

        static DWORDLONG getUsedMemory() 
        {
            return getTotalMemory() - getFreeMemory();
        }

        static double getPercentUsed() 
        {
            DWORDLONG totalMemory = getTotalMemory();
            if (totalMemory == 0) {
                return 0.0;
            }
            DWORDLONG usedMemory = getUsedMemory();
            return (double(usedMemory) / totalMemory) * 100;
        }
    };

    struct Utils
    {
        static void printInformation() 
        {
            std::string mUser = Util::combine("User: ", Util::getUsername());
            std::string mCPU = Util::combine("CPU: ", Graphics::CPU::getCPUName());
            std::string mGPU = Util::combine("GPU: ", Graphics::GPU::getUsedGraphicsCardName());
            std::string mMemory = Util::combine("Memory: ", Graphics::Memory::getMemoryStats());

            Console::info(mUser);
            Console::info(mCPU);
            Console::info(mGPU);
            Console::info(mMemory);
        }
    };
};