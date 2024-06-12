#include <cassert>
#include <cmath>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/interpreter/frame.hpp"

namespace chai::interpreter {

#define DO_NEXT_INS()                                                          \
    Instruction newIns =                                                       \
        decoder::parse(currentFrame_->func_.code[pc() / sizeof(bytecode_t)]);  \
    std::cout << "NEXT_INS: " << newIns.operation << " = " << OP_TO_STR[newIns.operation] << std::endl;                                                                           \
    (this->*HANDLER_ARR[newIns.operation])(newIns);

Executor::Executor(CodeManager *manager, memory::LinearBuffer &buffer)
    : codeManager_(manager), buffer_(buffer) {}

void Executor::init() {
    assert(currentFrame_ == nullptr); // No current frame
    currentFrame_ = new (memory::LinearAllocator<Frame>{buffer_}.allocate(1))
        Frame(nullptr, codeManager_->getStartFunc(), buffer_);
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

chsize_t &Executor::acc() { return acc_; }
chsize_t Executor::acc() const { return acc_; }

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
    (*currentFrame_)[ins.r1] = acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::add(Instruction ins) {
    acc() += (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::addi(Instruction ins) {
    acc() += codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::sub(Instruction ins) {
    acc() -= (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::subi(Instruction ins) {
    acc() -= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mul(Instruction ins) {
    acc() *= (*currentFrame_)[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::muli(Instruction ins) {
    acc() *= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::div(Instruction ins) {
    acc() =
        static_cast<chsize_t>(std::bit_cast<int64_t>(acc()) /
                              static_cast<int64_t>((*currentFrame_)[ins.r1]));
    advancePc();
    DO_NEXT_INS()
}
void Executor::divi(Instruction ins) {
    acc() = std::bit_cast<chsize_t>(
        static_cast<int64_t>(acc()) /
        static_cast<int64_t>(codeManager_->getCnst(ins.immidiate)));
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
    double res = std::bit_cast<double>((*currentFrame_)[ins.r1]) +
                 std::bit_cast<double>(acc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::addif(Instruction ins) {
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) +
                 std::bit_cast<double>(acc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subf(Instruction ins) {
    double res = std::bit_cast<double>(acc()) -
                 std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subif(Instruction ins) {
    double res = std::bit_cast<double>(acc()) -
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulf(Instruction ins) {
    double res = std::bit_cast<double>((*currentFrame_)[ins.r1]) *
                 std::bit_cast<double>(acc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulif(Instruction ins) {
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) *
                 std::bit_cast<double>(acc());
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divf(Instruction ins) {
    double res = std::bit_cast<double>(acc()) /
                 std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divif(Instruction ins) {
    double res = std::bit_cast<double>(acc()) /
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icprint(Instruction ins) {
    assert(acc() <= 0xFF);
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
    acc() = std::bit_cast<chsize_t>(std::sqrt(std::bit_cast<double>(acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::icsin(Instruction ins) {
    acc() = std::bit_cast<chsize_t>(std::sin(std::bit_cast<double>(acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::iccos(Instruction ins) {
    acc() = std::bit_cast<chsize_t>(std::cos(std::bit_cast<double>(acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::if_icmpeq(Instruction ins) {
    if (acc() == (*currentFrame_)[ins.r1]) {
        static_assert(sizeof(Immidiate) == sizeof(int16_t));
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpne(Instruction ins) {
    if (acc() != (*currentFrame_)[ins.r1]) {
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpgt(Instruction ins) {
    if (acc() > (*currentFrame_)[ins.r1]) {
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpge(Instruction ins) {
    if (acc() >= (*currentFrame_)[ins.r1]) {
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmplt(Instruction ins) {
    if (acc() < (*currentFrame_)[ins.r1]) {
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmple(Instruction ins) {
    if (acc() <= (*currentFrame_)[ins.r1]) {
        pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_acmpeq(Instruction ins) {
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when objects will be introduced in chai.
     */
    DO_NEXT_INS()
}
void Executor::if_acmpne(Instruction ins) {
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when chai objects will be introduced.
     */
    DO_NEXT_INS()
}
void Executor::cmpgf(Instruction ins) {
    double acc_f64 = std::bit_cast<double>(acc());
    double r1_f64 = std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = (acc_f64 >= r1_f64) ? static_cast<size_t>(acc_f64 != r1_f64)
                                : static_cast<size_t>(-1);
    advancePc();
    DO_NEXT_INS()
}
void Executor::cmplf(Instruction ins) {
    double acc_f64 = std::bit_cast<double>(acc());
    double r1_f64 = std::bit_cast<double>((*currentFrame_)[ins.r1]);
    acc() = acc() = (acc_f64 <= r1_f64) ? static_cast<size_t>(acc_f64 != r1_f64)
                                        : static_cast<size_t>(-1);
    advancePc();
    DO_NEXT_INS()
}
void Executor::g0t0(Instruction ins) {
    pc() += static_cast<int16_t>(ins.immidiate);
    DO_NEXT_INS()
}
void Executor::call(Instruction ins) {
    memory::LinearAllocator<Frame> allocator{buffer_};
    currentFrame_ = new (allocator.allocate(1))
        Frame(currentFrame_, codeManager_->getFunc(ins.immidiate), buffer_);
    currentFrame_->passArgs();
    pc() = 0;
    DO_NEXT_INS();
}
void Executor::newi64array(Instruction ins) {
    auto n = static_cast<int64_t>(acc());
    memory::LinearAllocator<int64_t> allocator{buffer_};
    assert(n >= 0);
    auto *arr = new (allocator.allocate(n)) int64_t[n]();
    acc() = reinterpret_cast<chsize_t>(arr);
    advancePc();
    DO_NEXT_INS();
}
void Executor::get_i64from_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<int64_t *>(acc());
    acc() = arr[i];
    advancePc();
    DO_NEXT_INS();
}

void Executor::set_i64in_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<int64_t *>(acc());
    arr[i] = static_cast<int64_t>((*currentFrame_)[ins.r2]);
    advancePc();
    DO_NEXT_INS();
}

void Executor::newf64array(Instruction ins) {
    auto n = static_cast<int64_t>(acc());
    memory::LinearAllocator<double> allocator{buffer_};
    assert(n >= 0);
    auto *arr = new (allocator.allocate(n)) double[n]();
    acc() = reinterpret_cast<chsize_t>(arr);
    advancePc();
    DO_NEXT_INS();
}
void Executor::get_f64from_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<double *>(acc());
    acc() = std::bit_cast<int64_t>(arr[i]);
    advancePc();
    DO_NEXT_INS();
}

void Executor::set_f64in_arr(Instruction ins) {
    auto i = static_cast<int64_t>((*currentFrame_)[ins.r1]);
    auto *arr = reinterpret_cast<double *>(acc());
    arr[i] = std::bit_cast<double>((*currentFrame_)[ins.r2]);
    advancePc();
    DO_NEXT_INS();
}

void Executor::string_print(Instruction ins) {
    const std::string &str = codeManager_->getStringByStringPoolPos(acc());
    std::cout << str << std::endl;
    advancePc();
    DO_NEXT_INS();
}

void Executor::string_concat(Instruction ins) {
    const std::string &str1 = codeManager_->getStringByStringPoolPos(acc());
    const std::string &str2 =
        codeManager_->getStringByStringPoolPos((*currentFrame_)[ins.r1]);
    std::string concated = str1 + str2;
    acc() =
        codeManager_->getCnst(codeManager_->addCnstString(std::move(concated)));
    advancePc();
    DO_NEXT_INS();
}

void Executor::string_len(Instruction ins) {
    acc() = codeManager_->getStringByStringPoolPos(acc()).size();
    advancePc();
    DO_NEXT_INS();
}
void Executor::string_slice(Instruction ins) {
    const std::string &str = codeManager_->getStringByStringPoolPos(acc());
    std::cout << "string_slice" << std::endl;
    acc() = codeManager_->getCnst(codeManager_->addCnstString(
        str.substr((*currentFrame_)[ins.r1],
                   (*currentFrame_)[ins.r2] - (*currentFrame_)[ins.r1])));
    advancePc();
    DO_NEXT_INS();
}

InvalidInstruction::InvalidInstruction(const char *msg) : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
