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

}; // namespace tff

#endif