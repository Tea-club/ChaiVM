#pragma once

#include "ChaiVM/interpreter/objects.hpp"

// forward declaration for executor due to cyclic dependency
namespace chai::interpreter {
class Executor;
}

namespace chai::interpreter {

using chai::interpreter::Executor;

class GarbageCollector {
public:
    GarbageCollector(Executor &exec) : exec_(exec) {}

    void collect();

private:
    void collectRoots();
    void mark();

    std::vector<interpreter::Object> roots_{};
    Executor &exec_;
};

} // namespace chai::interpreter
