#include "tff.h"

namespace tff {

BTS::Tryte _stack[stackLength];
BTS::Tryte *stack = &_stack[BTS::maxValue];

funcTree _funcTable[stackLength];
funcTree *funcTable = &_funcTable[BTS::maxValue];

}; // namespace tff