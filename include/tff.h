#ifndef TFF_H
#define TFF_H

#include "BTS.h"
#include "Function.h"
#include "Stream.h"

namespace tff {

constexpr int stackLength = 2 * BTS::maxValue + 1;

using funcTree = Function::FunctionNode *;
// 内存栈
extern BTS::Tryte *stack;
// 代码树表
extern funcTree *funcTable;

// 含有`2`的Tryte表达式包装
struct TryteExpr {
    TryteExpr() : value(0), suffix(0){};
    TryteExpr(const TryteExpr &_te) : value(_te.value), suffix(_te.suffix){};
    TryteExpr(const char *);
    TryteExpr(const int &_i) : value(_i), suffix(0){};
    TryteExpr(const int &_i, const size_t &_suf) : value(_i), suffix(_suf){};
    ~TryteExpr() = default;
    BTS::Tryte value; // 字面值
    size_t suffix;    // 后缀数
    char *repr();
    operator int();
    operator BTS::Tryte();
};

}; // namespace tff

#endif