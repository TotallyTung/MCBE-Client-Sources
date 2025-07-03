//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include <cstdint>
#include <string>
#include <Utils/MiscUtils/MathUtils.hpp>

namespace mce {
    enum class ImageFormat : int {
        None = 0x0,
        R8Unorm = 0x1,
        RGB8Unorm = 0x2,
        RGBA8Unorm = 0x3,
    };

    class Blob {
    public:
        using value_type = unsigned char;
        using size_type = size_t;
        using pointer = value_type*;
        using iterator = value_type*;
        using const_pointer = value_type const*;
        using const_iterator = value_type const*;

        using delete_function = void (*)(pointer);

        struct Deleter {
        public:
            delete_function mFn;

            [[nodiscard]] _CONSTEXPR23 Deleter() : mFn(Blob::defaultDeleter) {}

            [[nodiscard]] _CONSTEXPR23 Deleter(delete_function fn) : mFn(fn) {}

            void operator()(pointer x) const { mFn(x); }
        };

        using pointer_type = std::unique_ptr<value_type[], Deleter>;

        pointer_type mBlob{}; // this+0x0
        size_type    mSize{}; // this+0x10

        [[nodiscard]] _CONSTEXPR23 Blob() = default;

        [[nodiscard]] _CONSTEXPR23 Blob(std::span<value_type> s, Deleter deleter = {}) : mSize(s.size()) { // NOLINT
            mBlob = pointer_type(new value_type[mSize], deleter);
            std::copy(s.begin(), s.end(), mBlob.get());
        }

        [[nodiscard]] _CONSTEXPR23 pointer data() const { return mBlob.get(); }

        [[nodiscard]] _CONSTEXPR23 size_type size() const { return mSize; }

        [[nodiscard]] _CONSTEXPR23 std::span<value_type> view() const { return { data(), size() }; }

        //LL_CLANG_CEXPR Blob& operator=(Blob&&) noexcept = default;
        [[nodiscard]] _CONSTEXPR23 Blob(Blob&&) noexcept = default;

        [[nodiscard]] _CONSTEXPR23 Blob(Blob const& other) : Blob(other.view(), other.mBlob.get_deleter()) {}

        _CONSTEXPR23 Blob& operator=(Blob const& other) {
            if (this != &other) {
                mSize = other.mSize;
                mBlob = pointer_type(new value_type[mSize], other.mBlob.get_deleter());
                std::copy(other.data(), other.data() + mSize, mBlob.get());
            }
            return *this;
        }

        static Blob fromVector(std::vector<unsigned char> const& vec) {
            Blob blob;
            blob.mSize = vec.size();
            blob.mBlob = pointer_type(new value_type[blob.mSize], Deleter());
            std::copy(vec.begin(), vec.end(), blob.mBlob.get());
            return blob;

        }

    public:
        static inline void defaultDeleter(pointer ptr) { delete[] ptr; }
    };

    enum class ImageUsage : unsigned char {
        Unknown = 0x0,
        sRGB    = 0x1,
        Data    = 0x2,
    };

    struct Image {
    public:
        ImageFormat imageFormat{}; // this+0x0
        unsigned int        mWidth{};      // this+0x4
        unsigned int        mHeight{};     // this+0x8
        unsigned int        mDepth{};      // this+0xC
        ImageUsage  mUsage{};      // this+0x10
        Blob        mImageBytes;   // this+0x18

        [[nodiscard]] constexpr Image() = default;
        //LL_CLANG_CEXPR Image& operator=(Image&&) noexcept = default;
        [[nodiscard]] constexpr Image(Image&&) noexcept = default;
        //LL_CLANG_CEXPR Image& operator=(Image const&) noexcept = default;
        [[nodiscard]] constexpr Image(Image const&) noexcept = default;
    };


    struct UUID {
        uint64_t mLow;
        uint64_t mHigh;

        static UUID generate()
        {
            UUID uuid = {};
            uuid.mLow = rand();
            uuid.mHigh = rand();
            return uuid;
        }

        [[nodiscard]] std::string toString() const
        {
            // UUID parts
            uint32_t timeLow = (mLow & 0xFFFFFFFF);
            uint16_t timeMid = (mLow >> 32) & 0xFFFF;
            uint16_t timeHiAndVersion = ((mLow >> 48) & 0x0FFF) | (4 << 12); // Set version to 4
            uint16_t clkSeq = (mHigh & 0x3FFF) | 0x8000; // Set variant to 2
            uint16_t nodeHi = (mHigh >> 16) & 0xFFFF;
            uint32_t nodeLow = (mHigh >> 32);

            std::stringstream ss;
            ss << std::hex << std::setfill('0')
               << std::setw(8) << timeLow << '-'
               << std::setw(4) << timeMid << '-'
               << std::setw(4) << timeHiAndVersion << '-'
               << std::setw(4) << clkSeq << '-'
               << std::setw(4) << nodeHi
               << std::setw(8) << nodeLow;

            return ss.str();
        }

        [[nodiscard]] bool operator==(const UUID& other) const
        {
            return mLow == other.mLow && mHigh == other.mHigh;
        }

        [[nodiscard]] bool operator!=(const UUID& other) const
        {
            return !(*this == other);
        }

        [[nodiscard]] bool operator<(const UUID& other) const
        {
            return mLow < other.mLow || (mLow == other.mLow && mHigh < other.mHigh);
        }

        [[nodiscard]] bool operator>(const UUID& other) const
        {
            return mLow > other.mLow || (mLow == other.mLow && mHigh > other.mHigh);
        }

        [[nodiscard]] bool operator<=(const UUID& other) const
        {
            return mLow < other.mLow || (mLow == other.mLow && mHigh <= other.mHigh);
        }

        [[nodiscard]] bool operator>=(const UUID& other) const
        {
            return mLow > other.mLow || (mLow == other.mLow && mHigh >= other.mHigh);
        }

        [[nodiscard]] bool operator!() const
        {
            return mLow == 0 && mHigh == 0;
        }

        [[nodiscard]] explicit operator bool() const
        {
            return mLow != 0 || mHigh != 0;
        }

        static UUID fromString(const std::string& str);
    };

    struct Color {
        float r;
        float g;
        float b;
        float a;

        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
        Color() : r(0), g(0), b(0), a(0) {};
    };

    class ResourceLocation {
    public:
        uint64_t mType;              //0x0000
        std::string mFilePath = "";  //0x0008

        ResourceLocation(std::string filePath, bool external) {
            memset(this, 0, sizeof(ResourceLocation));
            this->mFilePath = std::move(filePath);
            if (external)
                this->mType = 2;
        };
    };

    class TexturePtr {
    public:
        PAD(0x10);
        std::shared_ptr<ResourceLocation> mTexture = nullptr;
    };
}
