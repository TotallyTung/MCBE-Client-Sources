#pragma once

namespace mce {
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
};