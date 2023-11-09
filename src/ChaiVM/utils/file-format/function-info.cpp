#include <fstream>

#include "ChaiVM/utils/file-format/function-info.hpp"

void FunctionInfo::dump(std::ofstream &ofs) const {
    ofs.write(reinterpret_cast<const char *>(&accessFlags_),
              sizeof(accessFlags_));
    ofs.write(reinterpret_cast<const char *>(&nameAndTypeIndex_),
              sizeof(nameAndTypeIndex_));
    ofs.write(reinterpret_cast<const char *>(&attsCount_), sizeof(attsCount_));
    ofs.write(reinterpret_cast<const char *>(&attNameIndex_),
              sizeof(attNameIndex_));
    ofs.write(reinterpret_cast<const char *>(&attLen_), sizeof(attLen_));
    ofs.write(reinterpret_cast<const char *>(&maxRegisters_),
              sizeof(maxRegisters_));
    ofs.write(reinterpret_cast<const char *>(&nargs_), sizeof(nargs_));
    ofs.write(reinterpret_cast<const char *>(&codeLen_), sizeof(codeLen_));
    for (const auto &ins : code_) {
        ofs.write(reinterpret_cast<const char *>(&ins),
                  sizeof(chai::bytecode_t));
    }
}
