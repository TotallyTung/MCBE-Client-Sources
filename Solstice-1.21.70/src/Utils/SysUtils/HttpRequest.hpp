#pragma once
//
// Created by vastrakai on 8/16/2024.
//
#include <functional>
#include <string>
#include <wininet.h>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DEL // DELETE is a reserved keyword :(
};


struct HttpResponseEvent {
public:
    void* mSender = nullptr;
    std::string mResponse;
    DWORD mStatusCode = 0;
    void* mOriginalRequest = nullptr;
};

class HttpRequest {
public:
    HttpMethod mMethod;
    std::string mUrl;
    std::string mBody;
    std::string mHeaders;
    std::function<void(HttpResponseEvent)> mCallback;
    bool mRequestSent = false;
    std::future<void> mFuture = std::future<void>();
    void* mSender = nullptr;

    // Normal legit user agent
    std::string mUserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:129.0) Gecko/20100101 Firefox/129.0";

    HttpRequest() = default;

    HttpRequest(const HttpMethod method, const std::string& url, const std::string& body, const std::string& headers, const std::function<void(HttpResponseEvent)>& callback, void* sender) :
        mMethod(method), mUrl(url), mBody(body), mHeaders(headers), mCallback(callback), mSender(sender) {}
    void sendAsync();
    HttpResponseEvent send();

    ~HttpRequest() {
        if (mFuture.valid()) {
            mFuture.wait();
        }
    }

    [[nodiscard]] bool isDone() const
    {
        return mRequestSent && mFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    // = operator
    explicit operator HttpRequest&() {
        return *this;
    }
};

