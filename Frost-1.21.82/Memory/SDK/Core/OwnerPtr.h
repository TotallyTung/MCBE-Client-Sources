#pragma once

template <typename T>
class OwnerPtr {
public:
    std::shared_ptr<T> mHandle;

    constexpr OwnerPtr() noexcept = default;

    constexpr ~OwnerPtr() = default;

    constexpr OwnerPtr(std::nullptr_t) noexcept {}

    template <class Y>
    constexpr OwnerPtr(std::shared_ptr<Y> const& ptr)
        requires(std::convertible_to<Y*, T*>)
    : mHandle(ptr) {}
    template <class Y>
    constexpr OwnerPtr(std::shared_ptr<Y>&& ptr) noexcept
        requires(std::convertible_to<Y*, T*>)
    : mHandle(std::move(ptr)) {}
    template <class Y>
    constexpr OwnerPtr(OwnerPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    : mHandle(other.mHandle) {}
    template <class Y>
    constexpr OwnerPtr(OwnerPtr<Y>&& other) noexcept
        requires(std::convertible_to<Y*, T*>)
    : mHandle(std::move(other.mHandle)) {}

    template <class Y>
    constexpr OwnerPtr& operator=(std::shared_ptr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        mHandle = other;
        return *this;
    }

    template <class Y>
    constexpr OwnerPtr& operator=(std::shared_ptr<Y>&& other) noexcept
        requires(std::convertible_to<Y*, T*>)
    {
        mHandle = std::move(other);
        return *this;
    }

    template <class Y>
    constexpr OwnerPtr& operator=(OwnerPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        if (this != &other) {
            mHandle = other.mHandle;
        }
        return *this;
    }

    template <class Y>
    constexpr OwnerPtr& operator=(OwnerPtr<Y>&& other) noexcept
        requires(std::convertible_to<Y*, T*>)
    {
        if (this != &other) {
            mHandle = std::move(other.mHandle);
        }
        return *this;
    }

    constexpr void reset() { mHandle.reset(); }

    constexpr operator bool() const { return mHandle != nullptr; } // NOLINT

    constexpr T* get() const { return mHandle.get(); }

    constexpr T& operator*() const { return *get(); }

    constexpr T* operator->() const { return get(); }
};