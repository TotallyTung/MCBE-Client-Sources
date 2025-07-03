//
// Created by vastrakai on 8/16/2024.
//

#include "HttpRequest.hpp"
#include <functional>
#include <string>
#include <wininet.h>

void HttpRequest::sendAsync()
{
    // This is so aids
    if (mRequestSent)
    {
        spdlog::warn(__FUNCTION__ " called on an already sent request!");
        return;
    }

    mRequestSent = true;

    mFuture = std::async(std::launch::async, [this]()
    {
        // Send da request
        HINTERNET hInternet = InternetOpenA(mUserAgent.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
        if (!hInternet) {
            mCallback({ mSender, "Failed to open the internet", 0, this });
            return;
        }

        HINTERNET hConnect = InternetOpenUrlA(hInternet, mUrl.c_str(), mHeaders.c_str(), mHeaders.size(), INTERNET_FLAG_RELOAD, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            mCallback({ mSender, "Failed to connect to the server", 0, this });
            return;
        }

        // Read the response
        char buffer[4096];
        DWORD bytesRead;
        std::string response;
        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            response.append(buffer, bytesRead);
        }

        // Get the status code
        DWORD statusCode;
        DWORD statusCodeSize = sizeof(statusCode);
        HttpQueryInfoA(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, nullptr);

        // Close the connection
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        // Call the callback
        mCallback({ mSender, response, statusCode, this });
    });
}

HttpResponseEvent HttpRequest::send() {
    // Initialize internet connection
    HINTERNET hInternet = InternetOpenA(mUserAgent.c_str(), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) {
        return { mSender, "Failed to open the internet", 0, this };
    }

    // Prepare the connection URL and handle
    HINTERNET hConnect = InternetOpenUrlA(hInternet, mUrl.c_str(), mHeaders.c_str(), mHeaders.size(), INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return { mSender, "Failed to connect to the server", 0, this };
    }

    // Read the response
    char buffer[4096];
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    // Get the status code
    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    HttpQueryInfoA(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, nullptr);

    // Close the connection handles
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    // Create and return the HttpResponseEvent
    return { mSender, response, statusCode, this };
}