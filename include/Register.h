#ifndef REGISTER_H
#define REGISTER_H

#include <functional>
#include <map>

namespace Register {

std::map<int, std::function<void *()>> m;

template <typename _T> void REG(int _k) {
    const std::function<void *()> _f = []() { return (void *)new _T; };
    m[_k] = f;
}

template <typename _base> _base *FAC(int _k) { return (_base *)m[_k](); }

}; // namespace Register

#endif