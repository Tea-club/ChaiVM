#include "ChaiVM/utils/file-format/klass-info.hpp"

namespace chai::utils::fileformat {

chai::interpreter::Immidiate KlassInfo::addField(FieldInfo field) {
    fields_.push_back(field);
    nFields_++;
    return (nFields_ - 1) * sizeof(chsize_t);
}

void KlassInfo::dump(std::ofstream &ofs) const {
    writeBytes(ofs, &name_);
    writeBytes(ofs, &nFields_);
    for (const auto &field : fields_) {
        writeBytes(ofs, &field.name_);
        writeBytes(ofs, &field.isObject_);
        writeBytes(ofs, &field.tagOrKlassNum_);
    }
}

} // namespace chai::utils::fileformat
