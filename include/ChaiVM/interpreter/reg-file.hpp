#pragma once

#include <array>
#include <cstdlib>
#include <limits>

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter {

class RegisterFile {
public:
    RegisterFile(chsize_t pc);
    chsize_t &operator[](int n) &;
    chsize_t &pc();
    chsize_t pc() const;
    chsize_t &acc();
    chsize_t acc() const;

    static constexpr size_t Size = std::numeric_limits<RegisterId>::max();

private:
    chsize_t acc_;
    chsize_t pc_;
    std::array<chsize_t, Size> registers_;
};

} // namespace chai::interpreter
