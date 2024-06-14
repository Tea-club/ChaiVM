#include "ChaiVM/interpreter/garbage-collector.hpp"
#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/interpreter/objects.hpp"

using namespace chai::interpreter;

void GarbageCollector::collect() {
    collectRoots();
    mark();
}
void GarbageCollector::collectRoots() {
    const interpreter::Frame *frame = exec_.getCurrentFrame();
    while (frame != nullptr) {
        for (int i = 0; i < frame->size(); ++i) {
            if (frame->isRegisterReference(i)) {
                chsize_t ref = frame->operator[](i);
                if (ref != CHAI_NULL) {
                    // todo: balls explosion alert!
                    roots_.push_back(Object(ref));
                }
            }
        }
        frame = frame->back();
    }
    if (exec_.isAccRef()) {
        chsize_t ref = exec_.acc();
        if (ref != CHAI_NULL) {
            roots_.push_back(Object(exec_.acc()));
        }
    }
}
void GarbageCollector::mark() {
    for (auto &root : roots_) {
        if (root.isMarked()) {
            std::cout << "balls explosion. shouldn't be marked." << std::endl;
            continue;
        }
        if (root.klassId() == OBJ_ARR_IMM) {
            markObjectArrays(root);
        } else {
            markObjects(root);
        }
    }
}
void GarbageCollector::markObjects(Object obj) {
    if (obj.isMarked()) {
        std::cout << "balls explosion. shouldn't be marked." << std::endl;
    }
}
void GarbageCollector::markObjectArrays(Object /*unused*/) {

}

void GarbageCollector::sweep() {}
