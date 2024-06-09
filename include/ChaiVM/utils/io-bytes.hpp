#pragma once

#include <fstream>

namespace chai::utils {

/**
 * Writes bytes of data of a given type from a given address to a given output
 * stream.
 *
 * @tparam T type of data.
 * @param os output stream.
 * @param pdata pointer to data.
 */
template <typename T> void writeBytes(std::ostream &os, const T* pdata) {
    os.write(reinterpret_cast<const char *>(pdata), sizeof(T));
}

/**
 * Reads bytes of data of a given type from a given output stream.
 *
 * @tparam T type of data.
 * @param is input stream.
 * @return
 */
template <typename T> T readBytes(std::istream &is) {
    T data;
    is.read(reinterpret_cast<char *>(&data), sizeof(T));
    return data;
}

}  // namespace chai::utils
