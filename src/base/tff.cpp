#include "tff.h"

namespace tff {

BTS::Tryte _stack[stackLength];
BTS::Tryte *stack = &_stack[BTS::maxValue];

FuncTree _funcTable[stackLength];
FuncTree *funcTable = &_funcTable[BTS::maxValue];

}; // namespace tff