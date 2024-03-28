#pragma once

#include <string>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/interpreter/instruction.hpp"

/**
 * Class that encapsulates chai::interpreter::Operation and provides some
 *  useful methods.
 */
class SmartOperation final {
public:
    SmartOperation(chai::interpreter::Operation val) : val_(val) {}
    explicit SmartOperation(const std::string &strOp) {
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
