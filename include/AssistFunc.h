#ifndef _ASSISTFUNC_H
#define _ASSISTFUNC_H

#include <iostream>

namespace AssistFunc {

// 编译期对2取指函数
constexpr int pow2(int x) {
    int ret = 1;
    for (int i = 0; i < x; i++)
        ret *= 2;
    return ret;
}

// 左值二进制写入
void binaryIn(std::ostream &ostr, auto &data) { ostr.write((char *)&data, sizeof(data)); }
// 右值二进制写入
void binaryIn(std::ostream &ostr, auto &&data) { ostr.write((char *)&data, sizeof(data)); }
// 二进制读出
template <typename _T> _T binaryOut(std::istream &istr) {
    _T data;
    istr.read((char *)&data, sizeof(data));
    return data;
}

}; // namespace AssistFunc

#endif