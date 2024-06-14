#pragma once

#include <bit>

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"
#include "ChaiVM/memory/traced-allocator.hpp"
#include "decoder.hpp"
#include "frame.hpp"
#include "garbage-collector.hpp"
#include "objects.hpp"

namespace chai::interpreter {

class Executor {
public:
    using Handler = void (Executor::*)(Instruction);

    Executor(CodeManager *manager, memory::LinearBuffer &framesBuffer,
             memory::LinearBuffer &primitivesBuffer,
             memory::TracedByteAllocator &objectsAllocator_);

    /**
     * Loads the first frame (public static void main).
     */
    void init();

    void run();

    chsize_t &acc();
    chsize_t acc() const;

    bool isAccRef() const;

    Frame const *getCurrentFrame() const;
    const CodeManager* getCodeManager() const;
    memory::TracedByteAllocator& getObjectAllocator();
    const GarbageCollector& getGC() const;

private:
    chsize_t &pc();
    chsize_t pc() const;
    inline void advancePc();
    void inv(Instruction ins);
    void nop(Instruction ins);
    void ret(Instruction ins);
    void mov(Instruction ins);
    void ldia(Instruction ins);
    void ldra(Instruction ins);
    void star(Instruction ins);
    // @todo #8:90m/DEV Check arithmetic instruction on overflow.
    void add(Instruction ins);
    void addi(Instruction ins);
    void sub(Instruction ins);
    void subi(Instruction ins);
    void mul(Instruction ins);
    void muli(Instruction ins);
    void div(Instruction ins);
    void divi(Instruction ins);
    void modi(Instruction ins);
    void mod(Instruction ins);
    void ldiaf(Instruction ins);
    void addf(Instruction ins);
    void addif(Instruction ins);
    void subf(Instruction ins);
    void subif(Instruction ins);
    void mulf(Instruction ins);
    void mulif(Instruction ins);
    void divf(Instruction ins);
    void divif(Instruction ins);
    void icprint(Instruction ins);
    void icscani(Instruction ins);
    void icscanf(Instruction ins);
    void icsqrt(Instruction ins);
    void icsin(Instruction ins);
    void iccos(Instruction ins);
    void if_icmpeq(Instruction ins);
    void if_icmpne(Instruction ins);
    void if_icmpgt(Instruction ins);
    void if_icmpge(Instruction ins);
    void if_icmplt(Instruction ins);
    void if_icmple(Instruction ins);
    void if_acmpeq(Instruction ins);
    void if_acmpne(Instruction ins);
    void if_null(Instruction ins);
    void cmpgf(Instruction ins);
    void cmplf(Instruction ins);
    void g0t0(Instruction ins);
    void call(Instruction ins);
    void newi64array(Instruction ins);
    void get_i64from_arr(Instruction ins);
    void set_i64in_arr(Instruction ins);
    void newf64array(Instruction ins);
    void get_f64from_arr(Instruction ins);
    void set_f64in_arr(Instruction ins);
    void new_ref_arr(Instruction ins);
    void get_ref_from_arr(Instruction ins);
    void set_ref_in_arr(Instruction ins);
    void string_print(Instruction ins);
    void string_concat(Instruction ins);
    void string_len(Instruction ins);
    void string_slice(Instruction ins);
    void alloc_ref(Instruction ins);
    void mov_ref(Instruction ins);
    void ldra_ref(Instruction ins);
    void star_ref(Instruction ins);
    void get_field(Instruction ins);
    void set_field(Instruction ins);

    static constexpr Handler HANDLER_ARR[] = {
        &Executor::inv,
        &Executor::nop,
        &Executor::ret,
        &Executor::mov,
        &Executor::ldia,
        &Executor::ldra,
        &Executor::star,
        &Executor::add,
        &Executor::addi,
        &Executor::sub,
        &Executor::subi,
        &Executor::mul,
        &Executor::muli,
        &Executor::div,
        &Executor::divi,
        &Executor::modi,
        &Executor::mod,
        &Executor::ldiaf,
        &Executor::addf,
        &Executor::addif,
        &Executor::subf,
        &Executor::subif,
        &Executor::mulf,
        &Executor::mulif,
        &Executor::divf,
        &Executor::divif,
        &Executor::icprint,
        &Executor::icscani,
        &Executor::icscanf,
        &Executor::icsqrt,
        &Executor::icsin,
        &Executor::iccos,
        &Executor::if_icmpeq,
        &Executor::if_icmpne,
        &Executor::if_icmpgt,
        &Executor::if_icmpge,
        &Executor::if_icmplt,
        &Executor::if_icmple,
        &Executor::if_acmpeq,
        &Executor::if_acmpne,
        &Executor::if_null,
        &Executor::cmpgf,
        &Executor::cmplf,
        &Executor::g0t0,
        &Executor::call,
        &Executor::newi64array,
        &Executor::get_i64from_arr,
        &Executor::set_i64in_arr,
        &Executor::newf64array,
        &Executor::get_f64from_arr,
        &Executor::set_f64in_arr,
        &Executor::new_ref_arr,
        &Executor::get_ref_from_arr,
        &Executor::set_ref_in_arr,
        &Executor::string_print,
        &Executor::string_concat,
        &Executor::string_len,
        &Executor::string_slice,
        &Executor::alloc_ref,
        &Executor::mov_ref,
        &Executor::ldra_ref,
        &Executor::star_ref,
        &Executor::get_field,
        &Executor::set_field,
    };

private:
    chsize_t acc_;
    bool isAccRef_ = false;
    CodeManager *codeManager_;
    memory::LinearBuffer &framesBuffer_;
    memory::LinearBuffer &primitivesBuffer_;
    memory::TracedByteAllocator &objectsAllocator_;
    Frame *currentFrame_ = nullptr;
    GarbageCollector gc_{*this};
};

inline void Executor::advancePc() { pc() += sizeof(bytecode_t); }

/**
 * @todo #8:30m>/DEV make this exception to take RegisterFile and return it's
 * dump in a string via `what()`
 */
class InvalidInstruction : public std::runtime_error {
public:
    InvalidInstruction(char const *msg);
    InvalidInstruction(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
