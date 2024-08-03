// Created by mars on 2/8/24

#ifndef MARS_BASE_ATOMIC_H
#define MARS_BASE_ATOMIC_H

#include <atomic>
#include <stdint.h>

namespace mars {
namespace detail {

template<typename T>
class AtomicIntegerT {
public:
    AtomicIntegerT() : m_value(0) {}

    T get() {
        return m_value.load();
    }

    T getAndAdd(T x) {
        return m_value.fetch_add(x);
    }

    T addAndGet(T x) {
        return m_value.fetch_add(x) + x;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    T decrementAndGet() {
        return addAndGet(-1);
    }

    void add(T x) {
        m_value.fetch_add(x);
    }

    void increment() {
        incrementAndGet();
    }

    void decrement() {
        decrementAndGet();
    }

    T getAndSet(T newValue) {
        return m_value.exchange(newValue);
    }

private:
    std::atomic<T> m_value;
};

}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

}

#endif // MARS_BASE_ATOMIC_H
