#pragma once

#include <cstddef>
#include <type_traits>

namespace chai::utils {

template <typename Type, size_t Bits, size_t From = 0>
Type ExtractBits(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    constexpr Type const Mask = ((static_cast<Type>(1) << Bits) - 1) << From;

    return (value & Mask) >> From;
}

template <typename PassT, typename RetT, size_t Bits, size_t From = 0>
RetT ExtractBits(PassT value) {
    return static_cast<RetT>(ExtractBits<PassT, Bits, From>(value));
}

} // namespace chai::utils
