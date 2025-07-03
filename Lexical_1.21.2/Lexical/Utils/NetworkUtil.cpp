#include "NetworkUtil.h"
#include "FileUtil.h"
#include <Windows.h>
#include <wininet.h>

bool NetworkUtil::canAccessInternet() {
	char url[] = "https://www.google.com/";
	return InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION, 0);
}

bool NetworkUtil::downloadFile(std::string name, std::string path, std::string url) {
	std::string filePath = path + name;

	HINTERNET webH = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;

	if (!webH)
		return false;

	urlFile = InternetOpenUrlA(webH, url.c_str(), NULL, NULL, NULL, NULL);

	if (!urlFile) {
		InternetCloseHandle(webH);
		return false;
	}

	std::ofstream outputFile(filePath, std::ios::binary);

	if (!outputFile.is_open()) {
		InternetCloseHandle(webH);
		InternetCloseHandle(urlFile);
		return false;
	}

	char buffer[2000];
	DWORD bytesRead;

	do
	{
		InternetReadFile(urlFile, buffer, 2000, &bytesRead);
		outputFile.write(buffer, bytesRead);
		memset(buffer, 0, 2000);
	} while (bytesRead);

	outputFile.close();
	InternetCloseHandle(webH);
	InternetCloseHandle(urlFile);

	return true;
}