#include <cassert>
#include <cmath>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/interpreter/frame.hpp"
#include "ChaiVM/interpreter/objects.hpp"

namespace chai::interpreter {

#define DO_NEXT_INS()                                                          \
    triggerGC();                                                               \
    Instruction newIns =                                                       \
        decoder::parse(currentFrame_->func_.code[pc() / sizeof(bytecode_t)]);  \
/*    std::cout << "Next inst: " << OP_TO_STR[newIns.operation] << ", imm = " << newIns.immidiate << std::endl; */                                      \
    (this->*HANDLER_ARR[newIns.operation])(newIns);

Executor::Executor(CodeManager *manager, memory::LinearBuffer &framesBuffer,
                   memory::LinearBuffer &primitivesBuffer,
                   memory::TracedByteAllocator &objectsAllocator_)
    : codeManager_(manager), framesBuffer_(framesBuffer),
      primitivesBuffer_(primitivesBuffer),
      objectsAllocator_(objectsAllocator_) {}

void Executor::init() {
    assert(currentFrame_ == nullptr); // No current frame
    currentFrame_ =
        new (memory::LinearAllocator<Frame>{framesBuffer_}.allocate(1))
            Frame(nullptr, codeManager_->getStartFunc(), framesBuffer_);
    pc() = 0;
}

void Executor::run() {
    init();
    assert(currentFrame_ != nullptr);
    DO_NEXT_INS()
}

chsize_t &Executor::pc() {
    assert(currentFrame_ != nullptr);
    return currentFrame_->pc_;
}
chsize_t Executor::pc() const {
    assert(currentFrame_ != nullptr);
    return currentFrame_->pc_;
}

chsize_t &Executor::acc() {
    isAccRef_ = false;
    return acc_;
}
chsize_t Executor::getAcc() const { return acc_; }

bool Executor::isAccRef() const { return isAccRef_; }

Frame const *Executor::getCurrentFrame() const { return this->currentFrame_; }

#pragma GCC diagnostic ignored "-Wunused-parameter"

void Executor::inv(Instruction ins) {
    throw InvalidInstruction("Invalid operation at pc: " +
                             std::to_string(pc()));
}
void Executor::nop(Instruction ins) {
    advancePc();
    DO_NEXT_INS()
}
void Executor::ret(Instruction ins) {
    Frame *buf = currentFrame_;
    currentFrame_ = currentFrame_->back();
    buf->~Frame();
    memory::LinearAllocator<Frame> allocator{framesBuffer_};
    allocator.deallocate(buf, 1);
    if (currentFrame_ != nullptr) {
        advancePc();
        DO_NEXT_INS()
    }
    return;
}
void Executor::mov(Instruction ins) {
    (*currentFrame_)[ins.r2] = (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldia(Instruction ins) {
    acc() = codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldra(Instruction ins) {
    acc() = (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::star(Instruction ins) {
    assert(isAccRef() == false);
    (*currentFrame_)[ins.r1] = getAcc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::add(Instruction ins) {
    assert(isAccRef() == false);
    acc() += (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::addi(Instruction ins) {
    assert(isAccRef() == false);
    acc() += codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::sub(Instruction ins) {
    assert(isAccRef() == false);
    acc() -= (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::subi(Instruction ins) {
    assert(isAccRef() == false);
    acc() -= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mul(Instruction ins) {
    assert(isAccRef() == false);
    acc() *= (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::muli(Instruction ins) {
    assert(isAccRef() == false);
    acc() *= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::div(Instruction ins) {
    assert(isAccRef() == false);
    acc() =
        static_cast<chsize_t>(std::bit_cast<int64_t>(getAcc()) /
                              static_cast<int64_t>((*currentFrame_)[ins.r1]));
    advancePc();
    DO_NEXT_INS()
}
void Executor::divi(Instruction ins) {
    assert(isAccRef() == false);
    acc() = std::bit_cast<chsize_t>(
        static_cast<int64_t>(getAcc()) /
        static_cast<int64_t>(codeManager_->getCnst(ins.immidiate)));
    advancePc();
    DO_NEXT_INS()
}
void Executor::modi(Instruction ins) {
    assert(isAccRef() == false);
    acc() = static_cast<chsize_t>(
        static_cast<int64_t>(getAcc()) %
        static_cast<int64_t>(codeManager_->getCnst(ins.immidiate)));
    advancePc();
    DO_NEXT_INS()
}
void Executor::mod(Instruction ins) {
    assert(isAccRef() == false);
    acc() =
        static_cast<chsize_t>(static_cast<int64_t>(getAcc()) %
                              static_cast<int64_t>((*currentFrame_)[ins.r1]));
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldiaf(Instruction ins) {
    double immd = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    acc() = std::bit_cast<chsize_t>(immd);
    advancePc();
    DO_NEXT_INS()
}

void Executor::addf(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>((*currentFrame_)[ins.r1]) +
                 std::bit_cast<double>(getAcc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::addif(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) +
                 std::bit_cast<double>(getAcc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subf(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(getAcc()) -
                 std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subif(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(getAcc()) -
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulf(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>((*currentFrame_)[ins.r1]) *
                 std::bit_cast<double>(getAcc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulif(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) *
                 std::bit_cast<double>(getAcc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divf(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(getAcc()) /
                 std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divif(Instruction ins) {
    assert(isAccRef() == false);
    double res = std::bit_cast<double>(getAcc()) /
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icprint(Instruction ins) {
    assert(isAccRef() == false);
    std::cout << static_cast<int64_t>(getAcc()) << " ";
    advancePc();
    DO_NEXT_INS()
}
void Executor::icscani(Instruction ins) {
    int64_t data;
    std::cin >> data;
    acc() = data;
    advancePc();
    DO_NEXT_INS()
}
void Executor::icscanf(Instruction ins) {
    double data;
    std::cin >> data;
    acc() = std::bit_cast<chsize_t>(data);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icsqrt(Instruction ins) {
    assert(isAccRef() == false);
    acc() = std::bit_cast<chsize_t>(std::sqrt(std::bit_cast<double>(getAcc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::icsin(Instruction ins) {
    assert(isAccRef() == false);
    acc() = std::bit_cast<chsize_t>(std::sin(std::bit_cast<double>(getAcc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::iccos(Instruction ins) {
    assert(isAccRef() == false);
    acc() = std::bit_cast<chsize_t>(std::cos(std::bit_cast<double>(getAcc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::if_icmpeq(Instruction ins) {
    assert(isAccRef() == false);
    static_assert(sizeof(Immidiate) == sizeof(int16_t));
    if (getAcc() == (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpne(Instruction ins) {
    assert(isAccRef() == false);
    if (getAcc() != (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpgt(Instruction ins) {
    assert(isAccRef() == false);
    if (getAcc() > (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpge(Instruction ins) {
    assert(isAccRef() == false);
    if (getAcc() >= (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmplt(Instruction ins) {
    assert(isAccRef() == false);
    if (getAcc() < (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmple(Instruction ins) {
    assert(isAccRef() == false);
    if (getAcc() <= (*currentFrame_)[ins.r1]) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_acmpeq(Instruction ins) {
    assert(isAccRef() == false);
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when objects will be introduced in chai.
     */
    DO_NEXT_INS()
}
void Executor::if_acmpne(Instruction ins) {
    assert(isAccRef() == false);
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when chai objects will be introduced.
     */
    DO_NEXT_INS()
}
void Executor::if_null(Instruction ins) {
    assert(isAccRef() == true);
    if (getAcc() == CHAI_NULL) {
        pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::cmpgf(Instruction ins) {
    assert(isAccRef() == false);
    double acc_f64 = std::bit_cast<double>(getAcc());
    double r1_f64 = std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = (acc_f64 >= r1_f64) ? static_cast<size_t>(acc_f64 != r1_f64)
                                : static_cast<size_t>(-1);
    advancePc();
    DO_NEXT_INS()
}
void Executor::cmplf(Instruction ins) {
    assert(isAccRef() == false);
    double acc_f64 = std::bit_cast<double>(getAcc());
    double r1_f64 = std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = acc() = (acc_f64 <= r1_f64) ? static_cast<size_t>(acc_f64 != r1_f64)
                                        : static_cast<size_t>(-1);
    advancePc();
    DO_NEXT_INS()
}
void Executor::g0t0(Instruction ins) {
    pc() += sizeof(bytecode_t) * static_cast<int16_t>(ins.immidiate);
    DO_NEXT_INS()
}
void Executor::call(Instruction ins) {
    memory::LinearAllocator<Frame> allocator{framesBuffer_};
    currentFrame_ = new (allocator.allocate(1)) Frame(
        currentFrame_, codeManager_->getFunc(ins.immidiate), framesBuffer_);
    currentFrame_->passArgs();
    pc() = 0;
    DO_NEXT_INS();
}
void Executor::newi64array(Instruction ins) {
    assert(isAccRef() == false);
    auto n = static_cast<int64_t>(getAcc());
    memory::LinearAllocator<int64_t> allocator{primitivesBuffer_};
    assert(n >= 0);
    auto *arr = new (allocator.allocate(n)) int64_t[n]();
    acc() = reinterpret_cast<chsize_t>(arr);
    advancePc();
    DO_NEXT_INS();
}
void Executor::get_i64from_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<int64_t *>(getAcc());
    acc() = arr[i];
    advancePc();
    DO_NEXT_INS();
}

void Executor::set_i64in_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<int64_t *>(getAcc());
    arr[i] = static_cast<int64_t>((*currentFrame_)[ins.r2]);
    advancePc();
    DO_NEXT_INS();
}

void Executor::newf64array(Instruction ins) {
    auto n = static_cast<int64_t>(getAcc());
    memory::LinearAllocator<double> allocator{primitivesBuffer_};
    assert(n >= 0);
    auto *arr = new (allocator.allocate(n)) double[n]();
    acc() = reinterpret_cast<chsize_t>(arr);
    advancePc();
    DO_NEXT_INS();
}
void Executor::get_f64from_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<double *>(getAcc());
    acc() = std::bit_cast<int64_t>(arr[i]);
    advancePc();
    DO_NEXT_INS();
}
void Executor::set_f64in_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<double *>(getAcc());
    arr[i] = std::bit_cast<double>((*currentFrame_)[ins.r2]);
    advancePc();
    DO_NEXT_INS();
}
void Executor::new_ref_arr(Instruction ins) {
    assert(isAccRef() == false);
    chsize_t len = getAcc();
    chsize_t num_bytes = ObjectArray::sizeOfObjectArray(len);
    auto *object_arr =
        new (objectsAllocator_.allocate(num_bytes)) uint8_t[num_bytes]();
    auto *pheader = reinterpret_cast<ObjectHeader *>(object_arr);
    auto *members =
        reinterpret_cast<chsize_t *>(object_arr + sizeof(ObjectHeader));
    pheader->size_ = num_bytes;
    pheader->klassId_ = OBJ_ARR_IMM;
    members[0] = len;
    for (int i = 1; i < len; ++i) {
        members[i] = CHAI_NULL;
    }
    acc() = std::bit_cast<chsize_t>(object_arr);
    isAccRef_ = true;
    advancePc();
    DO_NEXT_INS();
}
void Executor::get_ref_from_arr(Instruction ins) {
    assert(isAccRef() == true);
    assert(currentFrame_->isRegisterReference(ins.r1) == false);
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    ObjectHeader *header = std::bit_cast<ObjectHeader *>(getAcc());
    chsize_t *members = std::bit_cast<chsize_t *>(header + 1);
    chsize_t length = (header->size_ - sizeof(ObjectHeader)) / sizeof(chsize_t) - 1;
    assert(i >= 0);
    if (i >= length) {
        throw IndexOutOfBoundary("index " + std::to_string(i) +
                                 " is greater than array length " +
                                 std::to_string(length));
    }
    acc() = members[i + 1];
    isAccRef_ = true;
    advancePc();
    DO_NEXT_INS();
}
void Executor::set_ref_in_arr(Instruction ins) {
    assert(isAccRef() == true);
    assert(currentFrame_->isRegisterReference(ins.r1) == false);
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    assert(currentFrame_->isRegisterReference(ins.r2) == true);
    chsize_t new_ref = (*currentFrame_)[ins.r2];
    ObjectHeader *header = std::bit_cast<ObjectHeader *>(getAcc());
    chsize_t *members = std::bit_cast<chsize_t *>(header + 1);
    chsize_t length = (header->size_ - sizeof(ObjectHeader)) / sizeof(chsize_t) - 1;
    if (i < 0) {
        i += length;
    }
    if (i >= length) {
        throw IndexOutOfBoundary("index " + std::to_string(i) +
                                 " is greater than array length " +
                                 std::to_string(length));
    }
    members[i + 1] = new_ref;
    advancePc();
    DO_NEXT_INS();
}
void Executor::string_print(Instruction ins) {
    const std::string &str = codeManager_->getStringByStringPoolPos(getAcc());
    std::cout << str << std::endl;
    advancePc();
    DO_NEXT_INS();
}

void Executor::string_concat(Instruction ins) {
    const std::string &str1 = codeManager_->getStringByStringPoolPos(getAcc());
    const std::string &str2 =
        codeManager_->getStringByStringPoolPos((*currentFrame_)[ins.r1]);
    std::string concated = str1 + str2;
    acc() =
        codeManager_->getCnst(codeManager_->addCnstString(std::move(concated)));
    advancePc();
    DO_NEXT_INS();
}

void Executor::string_len(Instruction ins) {
    acc() = codeManager_->getStringByStringPoolPos(getAcc()).size();
    advancePc();
    DO_NEXT_INS();
}
void Executor::string_slice(Instruction ins) {
    const std::string &str = codeManager_->getStringByStringPoolPos(getAcc());
    acc() = codeManager_->getCnst(codeManager_->addCnstString(
        str.substr((*currentFrame_)[ins.r1],
                   (*currentFrame_)[ins.r2] - (*currentFrame_)[ins.r1])));
    advancePc();
    DO_NEXT_INS();
}
void Executor::alloc_ref(Instruction ins) {
    const Klass &klass = codeManager_->getKlass(ins.immidiate);
    assert(klass.instanceSize() > 0);
    auto *object = new (objectsAllocator_.allocate(klass.instanceSize()))
        uint8_t[klass.instanceSize()]();
    auto *pheader = reinterpret_cast<ObjectHeader *>(object);
    auto *fields = reinterpret_cast<chsize_t *>(object + sizeof(*pheader));
    pheader->size_ = klass.instanceSize();
    pheader->klassId_ = ins.immidiate;
    pheader->klassId_ = ins.immidiate;
    for (int i = 0; i < klass.nFields(); ++i) {
        assert(fields[i] == 0);
    }
    acc() = std::bit_cast<chsize_t>(object);
    isAccRef_ = true;
    advancePc();
    DO_NEXT_INS()
}
void Executor::mov_ref(Instruction ins) {
    std::cout << ins.operation << ": mov_ref is not implemented" << std::endl;
    assert(1 == 0);
}
void Executor::ldra_ref(Instruction ins) {
    acc() = (*currentFrame_)[ins.r1];
    assert(currentFrame_->isRegisterReference(ins.r1) == true);
    isAccRef_ = true;
    advancePc();
    DO_NEXT_INS()
}
void Executor::star_ref(Instruction ins) {
    assert(isAccRef() == true);
    (*currentFrame_)[ins.r1] = getAcc();
    currentFrame_->setRegisterIsRef(ins.r1, true);
    advancePc();
    assert(isAccRef() == true);
    DO_NEXT_INS()
}
void Executor::get_field(Instruction ins) {
    assert(isAccRef_ == true);
    Immidiate offset = ins.immidiate;
    ObjectHeader *header = std::bit_cast<ObjectHeader *>(getAcc());
    chsize_t *members = std::bit_cast<chsize_t *>(header + 1);
    assert(offset % sizeof(chsize_t) == 0);
    acc() = members[offset / sizeof(chsize_t)];
    if ((header->klassId_ != OBJ_ARR_IMM) &&
        (codeManager_->getKlass(header->klassId_)).fieldIsObject(offset)) {
        isAccRef_ = true;
    }
    advancePc();
    DO_NEXT_INS()
}
void Executor::set_field(Instruction ins) {
    assert(isAccRef() == true);
    Immidiate offset = ins.immidiate;
    ObjectHeader *header = std::bit_cast<ObjectHeader *>(getAcc());
    chsize_t *members = std::bit_cast<chsize_t *>(header + 1);
    assert(offset % sizeof(chsize_t) == 0);
    members[offset / sizeof(chsize_t)] = (*currentFrame_)[ins.r1];
    //    if ((codeManager_->getKlass(object.klassId())).fieldIsObject(offset))
    //    {
    //        assert(currentFrame_->isRegisterReference(r1) == true);
    //    } else {
    //        assert(currentFrame_->isRegisterReference(r1) == false);
    //    }
    assert((codeManager_->getKlass(header->klassId_)).fieldIsObject(offset) ==
           currentFrame_->isRegisterReference(ins.r1));
    assert(isAccRef() == true);
    advancePc();
    DO_NEXT_INS()
}
const CodeManager *interpreter::Executor::getCodeManager() const {
    return codeManager_;
}
memory::TracedByteAllocator &interpreter::Executor::getObjectAllocator() {
    return objectsAllocator_;
}
const GarbageCollector &interpreter::Executor::getGC() const {
    return gc_;
}
void interpreter::Executor::triggerGC() {
    if (static_cast<double>(objectsAllocator_.allocated()) >
        static_cast<double>(objectsAllocator_.size()) * 0.6) {
        gc_.collect();
    }
}

InvalidInstruction::InvalidInstruction(const char *msg) : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
