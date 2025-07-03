#pragma once

namespace mce {
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

        //CLASS_FIELD(std::unique_ptr<uint8_t*>, mData, 0x8);

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
                *this = Blob{ other };
            }
            return *this;
        }

        // This way is just funneh
        static Blob getBlob(Blob blob) {
            return blob;
        }

        static Blob fromVector(std::vector<unsigned char> const& vec) {
            Blob blob;
            blob.mSize = vec.size();
            blob.mBlob = pointer_type(new value_type[blob.mSize], Deleter());
            std::copy(vec.begin(), vec.end(), blob.mBlob.get());
            return blob;

        }
    public:
        static void defaultDeleter(pointer ptr) {
            delete[] ptr;
        }
    };
};