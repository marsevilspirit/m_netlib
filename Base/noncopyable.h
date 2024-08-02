#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

struct base_token {};

class noncopyable : base_token {
protected:
    noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

#endif // NONCOPYABLE_H
