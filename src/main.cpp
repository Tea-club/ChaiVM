#include "ChaiVM/interpreter/code-manager.hpp"

using namespace chai::interpreter;

void fillFileWithCode(const std::filesystem::path &path, const std::vector<chai::bytecode_t> &rawInstructions) {
    std::ofstream ofs(path, std::ios::binary | std::ios::out);
    if (ofs.good() && ofs.is_open()) {
        for(const auto& ins : rawInstructions) {
            ofs.write(reinterpret_cast<const char*>(&ins), sizeof(chai::bytecode_t));
        }
        ofs.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
}

uint8_t operation2opcode(Operation operation) { return (uint8_t)operation; }
chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2) {
    return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
}
chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
    return (operation2opcode(op)) |
           (static_cast<chai::bytecode_t>(imm) << 8);
}
chai::bytecode_t instr2Raw(Operation op) { return (operation2opcode(op)); }

int main() {
    std::cout << "Hello world" << std::endl;

    std::vector<chai::bytecode_t> defaultInstructions_ = {
        instr2Raw(Ldia, 6),    instr2Raw(Star, 2, 0), instr2Raw(Ldia, 8),
        instr2Raw(Star, 3, 0), instr2Raw(Ldra, 3, 0), instr2Raw(Mul, 2, 0),
        instr2Raw(Ret, 0, 0)};

    fillFileWithCode("./ex.chai", defaultInstructions_);

    CodeManager manager;
    manager.load("./ex.chai");
}