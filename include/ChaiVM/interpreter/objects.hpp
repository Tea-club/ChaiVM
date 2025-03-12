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

    /**
     * Flag that object still has references
     */
    bool isMarked_;
};

/**
 * Facade to manage object.
 * Note, it does not own header and fields.
 * @todo #97:30min Consider refactoring object to just cast it from raw pointer.
 *  It can be faster.
 */
class Object {

public:
    explicit inline Object(ObjectHeader *header, chsize_t *fields)
        : header_(header), members_(fields) {}

    /**
     * Ctor.
     * Create object from ref to object.
     * @param ref Ref to object (usually contains in register).
     */
    explicit inline Object(chsize_t ref)
        : header_(std::bit_cast<ObjectHeader *>(ref)),
          members_(std::bit_cast<chsize_t *>(header_ + 1)) {}

    /**
     * Get count of members.
     */
    chsize_t countMembers() const {
        return (header_->size_ - sizeof(ObjectHeader)) / sizeof(chsize_t);
    }

    /**
     * Get member.
     * @param offset Offset starting from {@link members_} (i.e. without
     * header).
     * @return Value of member.
     */
    const chsize_t &getMember(Immidiate offset) const {
        assert(offset % sizeof(chsize_t) == 0);
        return members_[offset / sizeof(chsize_t)];
    }

    /**
     * Set member.
     * @param offset Offset starting from {@link members_} (i.e. without
     * header).
     * @param value Value to be set.
     */
    void setMember(Immidiate offset, chsize_t value) const {
        assert(offset % sizeof(chsize_t) == 0);
        members_[offset / sizeof(chsize_t)] = value;
    }

    bool isMarked() const { return header_->isMarked_; }
    void mark() { header_->isMarked_ = true; }
    void unmark() { header_->isMarked_ = false; }
    Immidiate klassId() const { return header_->klassId_; }

protected:
    ObjectHeader *header_;

    /**
     * In case of ordinary object this is array of fields.
     * In case of object array this is array of refs.
     */
    chsize_t *members_;
};

class IndexOutOfBoundary : public std::runtime_error {
public:
    explicit IndexOutOfBoundary(const char *msg) : runtime_error(msg) {}
    IndexOutOfBoundary(const std::string &msg) : runtime_error(msg) {}
    const char *what() const noexcept override { return runtime_error::what(); }
};

/**
 * Convenient way to use array of objects object.
 * We consider size as the first member of object.
 */
class ObjectArray final : private Object {

public:
    /**
     * Ctor.
     * Create object from ref to object.
     * @param ref Ref to object (usually contains in register).
     */
    explicit ObjectArray(chsize_t ref) : Object(ref) {}

    /**
     * Ctor.
     * Create object array from object.
     * @param obj The object
     */
    explicit ObjectArray(Object obj) : Object(obj) {}

    chsize_t length() const { return Object::countMembers() - 1; }

    chai::chsize_t &operator[](int64_t i) & {
        if (i < 0) {
            i += length();
        }
        if (i >= length()) {
            throw IndexOutOfBoundary("index " + std::to_string(i) +
                                     " is greater than array length " +
                                     std::to_string(length()));
        }
        return Object::members_[i + 1];
    }

    const chsize_t &operator[](int64_t i) const & {
        assert(i >= 0);
        if (i >= length()) {
            throw IndexOutOfBoundary("index " + std::to_string(i) +
                                     " is greater than array length " +
                                     std::to_string(length()));
        }
        return Object::members_[i + 1];
    }

    /**
     * Calculate size (in bytes) of object array.
     * @param len Array length/
     * @return size in bytes.
     */
    static chsize_t sizeOfObjectArray(int64_t len) {
        return sizeof(ObjectHeader) + (1 + len) * sizeof(chsize_t);
    }
};

} // namespace chai::interpreter
