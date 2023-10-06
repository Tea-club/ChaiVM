#ifndef CHAIVM_NON_COPYABLE_HPP
#define CHAIVM_NON_COPYABLE_HPP

class INonCopyable {
public:
    INonCopyable(const INonCopyable &rhs) = delete;
    INonCopyable &operator=(const INonCopyable &rhs) = delete;
    virtual ~INonCopyable() = 0;
};

#endif // CHAIVM_NON_COPYABLE_HPP
