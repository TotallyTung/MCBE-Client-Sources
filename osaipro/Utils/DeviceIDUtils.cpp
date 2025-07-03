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
        widePath,  // �h���C�u�̃��C�h������p�X
        wideVolumeName,  // ���C�h������̃{�����[����
        MAX_PATH + 1,
        &serialNumber,
        &maxComponentLength,
        &fileSystemFlags,
        nullptr,
        0
    )) {
        delete[] wideVolumeName;  // �����������
        return std::to_string(serialNumber);
    }
    else {
        delete[] wideVolumeName;  // �����������
        return "Error: Unable to get DeviceID";
    }
}
