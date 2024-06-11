#pragma once

#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/file-format/function-info.hpp"
#include "ChaiVM/utils/file-format/klass-info.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

namespace chai::utils::fileformat {

/**
 * Class for generating .chai files.
 */
class ChaiFile {
public:
    ChaiFile(std::vector<chai::bytecode_t> &&instrs,
             std::vector<std::unique_ptr<Constant>> &&pool);

    ChaiFile();

    chai::interpreter::Immidiate addInstr(chai::bytecode_t raw);

    chai::interpreter::Immidiate addConst(std::unique_ptr<Constant> &&constant);

    void addWithConst(chai::interpreter::Operation op, int64_t data);
    void addWithConst(chai::interpreter::Operation op, double data);

    chai::bytecode_t getWithConst(chai::interpreter::Operation op,
                                  int64_t data);
    chai::bytecode_t getWithConst(chai::interpreter::Operation op, double data);

    /**
     * A more or less convenient way to add a function to the file
     * @param access_flags
     * @param name
     * @param descriptor like in jvm.
     * @param instrs vector of raw instructions
     * @param num_args Number of arguments of the function.
     * @param max_regs Number of registers to be allocated on the frame.
     * @return immidiate - Id of the function (ConstFuncNameAndType).
     */
    chai::interpreter::Immidiate
    addFunction(chai::interpreter::Immidiate access_flags,
                const std::string &name, const std::string &descriptor,
                const std::vector<chai::bytecode_t> &instrs, uint8_t num_args,
                uint8_t max_regs = 100);

    /**
     * What id will return addFunction if call it in the current state.
     * We need it to use the function_ref in creating the function (for example,
     * recursive functions).
     * @return id of next func_ref.
     */
    chai::interpreter::Immidiate nextFunc() const;

    /**
     * Add klass by name only.
     * @param name Klass name.
     * @return Number of klass.
     */
    chai::interpreter::Immidiate registerKlass(const std::string &name) {
        klasses_.push_back(
            KlassInfo{addConst(std::make_unique<ConstRawStr>(name)), 0,
                      std::vector<FieldInfo>{}});
        return klasses_.size() - 1;
    }

    /**
     * Add field to klass.
     * @param klass Number of klass.
     * @param name Name of the field.
     * @param isObject 0 if primitive, otherwise 1.
     * @param tagOrKlassNum 1 for i64, 2 for f64, or number of klass.
     * @return offset of the field.
     */
    chai::interpreter::Immidiate
    addField(chai::interpreter::Immidiate klass, const std::string &name,
             uint8_t isObject, chai::interpreter::Immidiate tagOrKlassNum) {
        chai::interpreter::Immidiate field_name =
            addConst(std::make_unique<ConstRawStr>(name));
        return klasses_[klass].addField(
            FieldInfo{field_name, isObject, tagOrKlassNum});
    }

    void toFile(const std::filesystem::path &path) const;

private:
    void dumpMainFunc(std::ofstream &ofs) const;

private:
    std::vector<chai::bytecode_t> rawInstrs_;
    std::vector<std::unique_ptr<Constant>> pool_;

    std::vector<KlassInfo> klasses_;

    /**
     * All functions except main.
     */
    std::vector<FunctionInfo> functions_;

    /**
     * @todo #94:90min Figure out why this is necessary and add commentary or
     * remove.
     */
    chai::interpreter::Immidiate constFuncNameAndTypeIndex_;

    /**
     * Number of "code" str in constant pool.
     */
    chai::interpreter::Immidiate codeAttStr_;
};

} // namespace chai::utils::fileformat
