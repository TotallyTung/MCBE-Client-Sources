#include "DeviceIDUtils.h"
#include <Windows.h>
#include <string>

std::string DeviceIDUtils::getDeviceID() {
    char volumeName[MAX_PATH + 1];
    DWORD serialNumber;
    DWORD maxComponentLength;
    DWORD fileSystemFlags;
    LPCWSTR widePath = L"C:\\";
    size_t convertedChars;
    size_t size = strlen(volumeName) + 1;
    wchar_t* wideVolumeName = new wchar_t[size];
    mbstowcs_s(&convertedChars, wideVolumeName, size, volumeName, _TRUNCATE);

    if (GetVolumeInformation(
        widePath,  // ドライブのワイド文字列パス
        wideVolumeName,  // ワイド文字列のボリューム名
        MAX_PATH + 1,
        &serialNumber,
        &maxComponentLength,
        &fileSystemFlags,
        nullptr,
        0
    )) {
        delete[] wideVolumeName;  // メモリを解放
        return std::to_string(serialNumber);
    }
    else {
        delete[] wideVolumeName;  // メモリを解放
        return "Error: Unable to get DeviceID";
    }
}
