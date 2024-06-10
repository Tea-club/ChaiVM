#include <fstream>

#include "ChaiVM/utils/file-format/function-info.hpp"
#include "ChaiVM/utils/io-bytes.hpp"

namespace chai::utils::fileformat {

void FunctionInfo::dump(std::ofstream &ofs) const {
    writeBytes(ofs, &accessFlags_);
    writeBytes(ofs, &nameAndTypeIndex_);
    writeBytes(ofs, &attsCount_);
    writeBytes(ofs, &attNameIndex_);
    writeBytes(ofs, &attLen_);
    writeBytes(ofs, &maxRegisters_);
    writeBytes(ofs, &nargs_);
    writeBytes(ofs, &codeLen_);
    for (const auto &ins : code_) {
        writeBytes(ofs, &ins);
    }
}

} // namespace chai::utils::fileformat
