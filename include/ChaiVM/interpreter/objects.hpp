#pragma once

#include "ChaiVM/interpreter/code-manager/klass.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include <cassert>

namespace chai::interpreter {

/**
 * Immidiate for klass of object array. Not elegant decision.
 * @todo #100:60min Figure out how to designate such a class.
 */
const Immidiate OBJ_ARR_IMM = UINT16_MAX - 1;

struct ObjectHeader {

    /**
     * Size of object. Array of objects is an object too.
     */
    chsize_t size_;

    /**
     * Number of klass in "klass pool".
     */
    Immidiate klassId_;
};

/**
 * Facade to manage object.
 * Note, it does not own header and fields.
 * @todo #97:30min Consider refactoring object to just cast it from raw pointer.
 *  It can be faster.
 */
class Object {

public:
    explicit Object(ObjectHeader *header, chsize_t *fields);

    /**
     * Ctor.
     * Create object from ref to object.
     * @param ref Ref to object (usually contains in register).
     */
    explicit Object(chsize_t ref);

    /**
     * Get count of members.
     */
    chsize_t countMembers() const;

    /**
     * Get member.
     * @param offset Offset starting from {@link members_} (i.e. without
     * header).
     * @return Value of member.
     */
    chsize_t getMember(Immidiate offset) const;

    /**
     * Set member.
     * @param offset Offset starting from {@link members_} (i.e. without
     * header).
     * @param value Value to be set.
     */
    void setMember(Immidiate offset, chsize_t value) const;

private:
    ObjectHeader *header_;

    /**
     * In case of ordinary object this is array of fields.
     * In case of object array this is array of refs.
     */
    chsize_t *members_;
};

} // namespace chai::interpreter
