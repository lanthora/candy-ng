// SPDX-License-Identifier: MIT
#ifndef CANDY_UTILITY_BYTESWAP_H
#define CANDY_UTILITY_BYTESWAP_H

#include "utility/type.h"
#include <cstddef>

namespace Candy {
template <typename T> static inline T byteswap(const T &input) {
    T output(0);
    const std::size_t size = sizeof(input);
    const uint8 *in = reinterpret_cast<const uint8 *>(&input);
    uint8 *out = reinterpret_cast<uint8 *>(&output);

    for (std::size_t i = 0; i < size; i++) {
        out[i] = in[size - i - 1];
    }

    return output;
}
} // namespace Candy

#endif
