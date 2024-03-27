#pragma once

#include <string>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/interpreter/instruction.hpp"

/**
 * Class that encapsulates chai::interpreter::Operation and provides some
 *  useful methods.
 */
class OperationWithInfo {
public:
    OperationWithInfo(chai::interpreter::Operation val) : val_(val) {}
    explicit OperationWithInfo(const std::string &strOp) {
        int ind = findString(strOp);
        if (ind == -1) {
            throw std::invalid_argument(
                "There is no enum value corresponding to the string: \"" +
                strOp + "\"");
        }
        val_ = static_cast<chai::interpreter::Operation>(ind);
    }

    std::string_view toString() const {
        return chai::interpreter::OP_TO_STR[val_];
    }
    chai::interpreter::OperationFormat format() {
        return chai::interpreter::OP_TO_FORMAT[val_];
    }

    constexpr operator chai::interpreter::Operation() const { return val_; }
    constexpr auto operator<=>(const OperationWithInfo &rhs) const = default;
    constexpr auto operator==(const chai::interpreter::Operation &rhs) const {
        return this->val_ == rhs;
    }

private:
    constexpr int findString(const std::string &strOp) {
        for (int i = 0; i < sizeof(chai::interpreter::OP_TO_STR) /
                                sizeof(chai::interpreter::OP_TO_STR[0]);
             i++) {
            if (strOp == chai::interpreter::OP_TO_STR[i]) {
                return i;
            }
        }
        return -1;
    }

    chai::interpreter::Operation val_ = chai::interpreter::Inv;
};
