#include "NetworkUtil.h"
#include "FileUtil.h"
#include <Windows.h>
#include <wininet.h>
#include <Windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

bool NetworkUtil::canAccessInternet() {
	char url[] = "https://www.google.com/";
	return InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION, 0);
}
bool NetworkUtil::sendPostRequest(const std::string& url, const std::string& data, const std::string& headers) {
    HINTERNET hSession = nullptr;
    HINTERNET hConnect = nullptr;
    HINTERNET hRequest = nullptr;
    bool result = false;
    try {
        URL_COMPONENTS urlComp;
        ZeroMemory(&urlComp, sizeof(urlComp));
        urlComp.dwStructSize = sizeof(urlComp);
        urlComp.dwSchemeLength = (DWORD)-1;
        urlComp.dwHostNameLength = (DWORD)-1;
        urlComp.dwUrlPathLength = (DWORD)-1;
        urlComp.dwExtraInfoLength = (DWORD)-1;
        if (!WinHttpCrackUrl(std::wstring(url.begin(), url.end()).c_str(), (DWORD)url.length(), 0, &urlComp)) {
            throw std::runtime_error("Failed to crack URL");
        }
        std::wstring hostName(urlComp.lpszHostName, urlComp.dwHostNameLength);
        std::wstring urlPath(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);
        hSession = WinHttpOpen(L"HWID Verifier/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) throw std::runtime_error("Failed to open session");
        hConnect = WinHttpConnect(hSession, hostName.c_str(),
            urlComp.nPort, 0);
        if (!hConnect) throw std::runtime_error("Failed to connect");
        hRequest = WinHttpOpenRequest(hConnect, L"POST",
            urlPath.c_str(),
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE);
        if (!hRequest) throw std::runtime_error("Failed to open request");
        if (!headers.empty()) {
            if (!WinHttpAddRequestHeaders(hRequest,
                std::wstring(headers.begin(), headers.end()).c_str(),
                (DWORD)-1L,
                WINHTTP_ADDREQ_FLAG_ADD)) {
                throw std::runtime_error("Failed to add headers");
            }
        }
        if (!WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, (LPVOID)data.c_str(),
            (DWORD)data.size(),
            (DWORD)data.size(), 0)) {
            throw std::runtime_error("Failed to send request");
        }
        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            throw std::runtime_error("Failed to receive response");
        }
        DWORD statusCode = 0;
        DWORD size = sizeof(statusCode);
        if (!WinHttpQueryHeaders(hRequest,
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode, &size, WINHTTP_NO_HEADER_INDEX)) {
            throw std::runtime_error("Failed to query status code");
        }
        result = (statusCode == 200);
    }
    catch (const std::exception& e) {
    }
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return result;
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