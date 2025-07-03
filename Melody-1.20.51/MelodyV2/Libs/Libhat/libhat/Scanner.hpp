#pragma once
#include <immintrin.h> // Include for SIMD intrinsics
#include <algorithm>
#include <execution>
#include <utility>
#include <cstdint>
#include <iostream>
#include <array>
#include "Concepts.hpp"
#include "Defines.hpp"
#include "Process.hpp"
#include "Signature.hpp"
namespace hat {

    class scan_result {
        using rel_t = int32_t;
    public:
        constexpr scan_result() : result(nullptr) {}
        constexpr scan_result(std::nullptr_t) : result(nullptr) {}
        constexpr scan_result(const std::byte* result) : result(result) {}

        template<std::integral Int>
        [[nodiscard]] constexpr Int read(size_t offset) const {
            return *reinterpret_cast<const Int*>(result + offset);
        }

        template<std::integral Int, typename ArrayType>
        [[nodiscard]] constexpr size_t index(size_t offset) const {
            return read<Int>(offset) / sizeof(ArrayType);
        }

        [[nodiscard]] constexpr const std::byte* rel(size_t offset) const {
            return has_result() ? result + read<rel_t>(offset) + offset + sizeof(rel_t) : nullptr;
        }

        [[nodiscard]] constexpr bool has_result() const {
            return result != nullptr;
        }

        [[nodiscard]] constexpr const std::byte* operator*() const {
            return result;
        }

        [[nodiscard]] constexpr const std::byte* get() const {
            return result;
        }
    private:
        const std::byte* result;
    };

    enum class scan_alignment {
        X1 = 1,
        X16 = 16
    };

    namespace detail {

        enum class scan_mode {
            FastFirst,
            SSE,
            AVX2,
            AVX512,

            Single = FastFirst
        };

        template<scan_alignment alignment>
        inline constexpr auto alignment_stride = static_cast<std::underlying_type_t<scan_alignment>>(alignment);

        template<std::integral type, scan_alignment alignment, auto stride = alignment_stride<alignment>>
        inline consteval auto create_alignment_mask() {
            constexpr size_t numBits = sizeof(type) * 8;
            type mask = 0;
            constexpr size_t numIterations = numBits / stride;
            for (size_t i = 0; i < numIterations; ++i) {
                mask |= static_cast<type>(1) << (i * stride);
            }
            return mask;
        }

        template<scan_alignment alignment, auto stride = alignment_stride<alignment>>
        inline const std::byte* next_boundary_align(const std::byte* ptr) {
            if constexpr (stride == 1) {
                return ptr;
            }
            else {
                uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
                uintptr_t alignedAddr = (addr + (stride - 1)) & ~(stride - 1);
                return reinterpret_cast<const std::byte*>(alignedAddr);
            }
        }

        template<scan_alignment alignment, auto stride = alignment_stride<alignment>>
        inline const std::byte* prev_boundary_align(const std::byte* ptr) {
            if constexpr (stride == 1) {
                return ptr;
            }
            else {
                uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
                uintptr_t alignedAddr = (addr & ~(stride - 1));
                return reinterpret_cast<const std::byte*>(alignedAddr);
            }
        }



        template<scan_mode, scan_alignment>
        scan_result find_pattern(const std::byte* begin, const std::byte* end, signature_view signature);

        template<scan_alignment alignment>
        scan_result find_pattern(const std::byte* begin, const std::byte* end, signature_view signature);

        template<>
        inline constexpr scan_result find_pattern<scan_mode::FastFirst, scan_alignment::X1>(const std::byte* begin, const std::byte* end, signature_view signature) {
            const auto firstByte = *signature[0];
            const auto scanEnd = end - signature.size() + 1;

            for (auto i = begin; i != scanEnd; ++i) {
                if (*i == firstByte) {
                    auto match = true;
                    for (size_t j = 1; j < signature.size(); ++j) {
                        if (signature[j].has_value() && i[j] != *signature[j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        return i;
                    }
                }
            }
            return nullptr;
        }

        template<>
        inline scan_result find_pattern<scan_mode::FastFirst, scan_alignment::X16>(const std::byte* begin, const std::byte* end, signature_view signature) {
            const auto firstByte = *signature[0];

            const auto scanBegin = next_boundary_align<scan_alignment::X16>(begin);
            const auto scanEnd = prev_boundary_align<scan_alignment::X16>(end - signature.size() + 1);
            if (scanBegin >= scanEnd) {
                return {};
            }

            for (auto i = scanBegin; i != scanEnd; i += 16) {
                if (*i == firstByte) {
                    auto match = true;
                    for (size_t j = 1; j < signature.size(); ++j) {
                        if (signature[j].has_value() && i[j] != *signature[j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        return i;
                    }
                }
            }
            return nullptr;
        }

    }

    template<scan_alignment alignment = scan_alignment::X1>
    scan_result find_pattern(
        signature_view      signature,
        process::module_t   mod = process::get_process_module()
    );

    template<scan_alignment alignment = scan_alignment::X1>
    scan_result find_pattern(
        signature_view      signature,
        std::string_view    section,
        process::module_t   mod = process::get_process_module()
    );

    template<scan_alignment alignment = scan_alignment::X1, detail::byte_iterator Iter>
    constexpr scan_result find_pattern(
        Iter            beginIt,
        Iter            endIt,
        signature_view  signature
    ) {
        size_t offset = 0;
        for (const auto& elem : signature) {
            if (elem.has_value()) {
                break;
            }
            offset++;
        }
        signature = signature.subspan(offset);

        const auto begin = std::to_address(beginIt) + offset;
        const auto end = std::to_address(endIt);
        if (begin >= end || end - begin < signature.size()) {
            return nullptr;
        }

        // Convert signature to contiguous memory for faster search
        std::vector<std::byte> patternBytes;
        patternBytes.reserve(signature.size());
        for (const auto& elem : signature) {
            if (elem.has_value()) {
                patternBytes.push_back(*elem);
            }
        }

        // Use std::search for optimized searching
        auto resultIt = std::search(begin, end, patternBytes.begin(), patternBytes.end());
        if (resultIt != end) {
            return resultIt - begin;
        }

        return nullptr;
    }
}

namespace hat::experimental {

    enum class compiler_type {
        MSVC,
        GNU
    };

    template<compiler_type compiler>
    scan_result find_vtable(
        const std::string& className,
        process::module_t   mod = process::get_process_module()
    );
}