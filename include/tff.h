#ifndef TFF_H
#define TFF_H

#include "BTS.h"
#include "Function.h"

namespace tff {

constexpr int stackLength = 2 * BTS::maxValue + 1;

BTS::Tryte _stack[stackLength];
auto stack = &_stack[BTS::maxValue];

}; // namespace tff

#endif