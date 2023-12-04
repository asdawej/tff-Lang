#ifndef TFF_H
#define TFF_H

#include "BTS.h"
#include "Function.h"
#include "Stream.h"
#include "std/Register.h"

namespace tff {

constexpr int stackLength = 2 * BTS::maxValue + 1;

using FuncTree = Function::FunctionNode *; // 代码树

// 内存栈
inline BTS::Tryte *stack = &(BTS::Tryte[stackLength]){}[BTS::maxValue];

// 代码树表
inline FuncTree *funcTable = &(FuncTree[stackLength]){}[BTS::maxValue];

enum struct FuncStatus { normal, fork, lock };

// 代码树线程状态
inline FuncStatus *funcStatusTable = &(FuncStatus[stackLength]){}[BTS::maxValue];

}; // namespace tff

#endif