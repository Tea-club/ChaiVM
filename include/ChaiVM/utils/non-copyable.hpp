#pragma once

class INonCopyable {
protected:
    INonCopyable() {}
public:
    INonCopyable(const INonCopyable &rhs) = delete;
    INonCopyable &operator=(const INonCopyable &rhs) = delete;
};
