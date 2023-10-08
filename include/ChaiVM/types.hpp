#ifndef CHAIVM_TYPES_HPP
#define CHAIVM_TYPES_HPP

#include <cstdint>

namespace chai {
using chsize_t = uint64_t;

/**
 * Type of native instruction - 32 bits.
 */
using chword_t = uint32_t;
} // namespace chai

#endif // CHAIVM_TYPES_HPP
