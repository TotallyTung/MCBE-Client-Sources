//
// Created by vastrakai on 7/26/2024.
//

#include "ChatGPT.hpp"
#include <wininet.h>


#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

std::string ChatGPT::getResponse(std::string input)
{
    // {"prompt":"balls","options":{},"model":"gpt-3.5-turbo","OPENAI_API_KEY":"sk-AItianhuFreeForEveryone","systemMessage":"You are an AI assistant, a large language model trained. Follow the user's instructions carefully. Respond using markdown.","temperature":0.8,"top_p":1}
    // Send a post request to the AI server with the input
    // Get the response from the server and return it
    HINTERNET internet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:128.0) Gecko/20100101 Firefox/128.0", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (internet == NULL)
    {
        spdlog::error("[ChatGPT] Failed to open internet connection");
        return "";
    }

    std::string requestBody = R"({"prompt":")" + input + R"(","options":{},"model":"gpt-3.5-turbo","OPENAI_API_KEY":"sk-AItianhuFreeForEveryone","systemMessage":"You are an AI assistant, a large language model trained. Follow the user's instructions carefully. Respond using markdown.","temperature":0.8,"top_p":1})";

    /*const wchar_t* requestHeaders = L"Accept: application/json, text/plain, #1#*\r\n"
    L"Accept-Encoding: gzip, deflate, br\r\n"
    L"Accept-Language: en-US,en;q=0.9\r\n"
    L"Cache-Control: no-cache\r\n"
    L"Content-Type: application/json\r\n"
    L"Origin: https://www.aitianhu.com\r\n"
    L"Pragma: no-cache\r\n"
    L"Referer: https://www.aitianhu.com/\r\n"
    L"Referrer-Policy: strict-origin-when-cross-origin\r\n"
    L"Sec-Ch-Ua: \"Not/A)Brand\";v=\"99\", \"Google Chrome\";v=\"115\", \"Chromium\";v=\"115\"\r\n"
    L"Sec-Ch-Ua-Mobile: ?0\r\n"
    L"Sec-Ch-Ua-Platform: \"Windows\"\r\n"
    L"Sec-Fetch-Dest: empty\r\n"
    L"Sec-Fetch-Mode: cors\r\n"
    L"Sec-Fetch-Site: same-origin\r\n"
    L"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36\r\n";*/
    std::string headerStr = "Accept: application/json, text/plain, */*\r\n" +
        std::string("Accept-Language: en-US,en;q=0.9\r\n") +
        std::string("Cache-Control: no-cache\r\n") +
        std::string("Content-Type: application/json\r\n") +
        std::string("Origin: https://www.aitianhu.com\r\n") +
        std::string("Pragma: no-cache\r\n") +
        std::string("Referer: https://www.aitianhu.com/\r\n") +
        std::string("Referrer-Policy: strict-origin-when-cross-origin\r\n") +
        std::string("Sec-Ch-Ua: \"Not;A)Brand\";v=\"99\", \"Google Chrome\";v=\"115\", \"Chromium\";v=\"115\"\r\n") +
        std::string("Sec-Ch-Ua-Mobile: ?0\r\n") +
        std::string("Sec-Ch-Ua-Platform: \"Windows\"\r\n") +
        std::string("Sec-Fetch-Dest: empty\r\n") +
        std::string("Sec-Fetch-Mode: cors\r\n") +
        std::string("Sec-Fetch-Site: same-origin\r\n") +
        std::string("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36\r\n");

    // Assuming aiUrl is the full URL including protocol, host, and path
    URL_COMPONENTSA urlComponents = {0};
    urlComponents.dwStructSize = sizeof(urlComponents);

    std::string aiUrl = "http://yx5ye3.aitianhu.com/api/please-donot-reverse-engineering-me-thank-you";

    if (!InternetCrackUrlA(aiUrl.c_str(), 0, 0, &urlComponents))
    {
        spdlog::error("[ChatGPT] Failed to parse URL");
        InternetCloseHandle(internet);
        return "";
    }

    std::string host = urlComponents.lpszHostName;

    HINTERNET connect = InternetConnectA(internet, host.c_str(), urlComponents.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (connect == NULL)
    {
        spdlog::error("[ChatGPT] Failed to connect to server");
        InternetCloseHandle(internet);
        return "";
    }

    HINTERNET request = HttpOpenRequestA(connect, "POST", urlComponents.lpszUrlPath, NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (request == NULL)
    {
        spdlog::error("[ChatGPT] Failed to open HTTP request");
        InternetCloseHandle(connect);
        InternetCloseHandle(internet);
        return "";
    }

    if (!HttpSendRequestA(request, headerStr.c_str(), static_cast<DWORD>(headerStr.length()), const_cast<char*>(requestBody.c_str()), static_cast<DWORD>(requestBody.length())))
    {
        spdlog::error("[ChatGPT] Failed to send request");
        InternetCloseHandle(request);
        InternetCloseHandle(connect);
        InternetCloseHandle(internet);
        return "";
    }

    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    if (!HttpQueryInfoA(request, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL))
    {
        spdlog::error("[ChatGPT] Failed to get status code");
        InternetCloseHandle(request);
        InternetCloseHandle(connect);
        InternetCloseHandle(internet);
        return "";
    }

    if (statusCode != 200)
    {
        spdlog::error("[ChatGPT] Server returned status code: {}", statusCode);
    }

    std::string response;
    char buffer[4096];
    DWORD bytesRead = 0;
    while (InternetReadFile(request, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(request);
    InternetCloseHandle(connect);
    InternetCloseHandle(internet);

    spdlog::info("[ChatGPT] Response: {}", response);
    response = StringUtils::fromBase64(response);

    // Get the last response from the server (last newline)
    size_t lastNewline = response.find_last_of('\n');
    if (lastNewline != std::string::npos)
    {
        response = response.substr(lastNewline + 1);
    }

    return response;

}
