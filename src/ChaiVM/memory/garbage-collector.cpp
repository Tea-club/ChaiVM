#include "ChaiVM/memory/garbage-collector.hpp"
#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::memory;

void GarbageCollector::collect() {
    collectRoots();
}
void GarbageCollector::collectRoots() {
    const interpreter::Frame* frame = exec_.getCurrentFrame();
    while (frame != nullptr) {
        for (int i = 0; i < frame->size(); ++i) {
            if (frame->isRegisterReference(i)) {
                //todo: balls explosion alert!
                roots_.push_back(reinterpret_cast<interpreter::Object*>(frame->operator[](i)));
            }
            frame = frame->back();
        }
    }
    if (exec_.isAccRef()) {
        roots_.push_back(reinterpret_cast<interpreter::Object*>(exec_.acc()));
    }
}
void GarbageCollector::mark() {

}
