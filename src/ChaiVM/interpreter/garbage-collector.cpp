#include "ChaiVM/interpreter/garbage-collector.hpp"
#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/interpreter/objects.hpp"

using namespace chai::interpreter;

void GarbageCollector::collect() {
    collectRoots();
    mark();
    sweep();
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
    obj.mark();
    std::vector<Field> obj_fields = exec_.getCodeManager()->getKlass(obj.klassId()).fields_;
    for (size_t i = 0; i < obj_fields.size(); i++) {
        if (obj_fields[i].isObject_) {
            // @todo #111:60min check if member references to array
            chsize_t member_ref = obj.getMember(i);
            if (member_ref != CHAI_NULL) {
                markObjects(Object{member_ref});
            }
        }
    }
}

void GarbageCollector::markObjectArrays(Object obj) {
    if (obj.isMarked()) {
        std::cout << "balls explosion. shouldn't be marked." << std::endl;
    }
    obj.mark();
    ObjectArray arr{obj};
    for (int64_t i = 0; i < arr.length(); ++i) {
        chsize_t elem_ref = arr[i];
        if (elem_ref != CHAI_NULL) {
            // @todo #111:60min check if elem references to array
            markObjects(Object{elem_ref});
        }
    }
}

void GarbageCollector::sweep() {
    memory::TracedByteAllocator& allocator = exec_.getObjectAllocator();
    for (auto& alloc_info : allocator.allocations()) {
        void* ptr = alloc_info.ptr;
        Object obj{reinterpret_cast<chsize_t>(ptr)};
        if (obj.isMarked()) {
            obj.unmark();
            continue;
        }
        allocator.deallocate(ptr, alloc_info.size);
        // @todo #111:60min to remove freed allocation here instead of additional loop
        alloc_info.isFree = true;
    }
    auto &allocations = allocator.allocations();
    std::list<memory::AllocationInfo>::iterator iter = allocations.begin();
    while (iter != allocations.end()) {
        if (iter->isFree) {
            iter = allocations.erase(iter);
        } else {
            iter++;
        }
    }
}
