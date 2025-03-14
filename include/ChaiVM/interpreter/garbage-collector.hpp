#pragma once

#include "ChaiVM/interpreter/objects.hpp"
#include <unordered_set>

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
    const std::vector<interpreter::Object> &getRoots() const;

private:
    void collectRoots();
    void mark();
    void markObjects(Object obj);
    void markObjectArrays(Object obj);
    void sweep();

    std::vector<interpreter::Object> roots_{};
    std::unordered_set<chsize_t> markedRefs_{};
    Executor &exec_;
};

} // namespace chai::interpreter
