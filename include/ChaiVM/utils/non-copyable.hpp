#pragma once

class INonCopyable {
public:
    INonCopyable(const INonCopyable &rhs) = delete;
    INonCopyable &operator=(const INonCopyable &rhs) = delete;
    virtual ~INonCopyable() = 0;
};
