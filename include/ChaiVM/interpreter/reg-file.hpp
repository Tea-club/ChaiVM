#ifndef CHAIVM_REG_FILE_HPP
#define CHAIVM_REG_FILE_HPP

#include <array>
#include <cstdlib>
#include <limits>

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter {

class RegisterFile {
public:
    chsize_t &operator[](int n) &;
    chsize_t getAcc() const;
    void setAcc(chsize_t value);
    chsize_t getPc() const;
    void setPc(chsize_t value);

    static constexpr size_t Size = std::numeric_limits<RegisterId>::max();

private:
    chsize_t acc_;
    chsize_t pc_ = 0;
    std::array<chsize_t, Size> registers_;
};

} // namespace chai::interpreter

#endif // CHAIVM_REG_FILE_HPP
