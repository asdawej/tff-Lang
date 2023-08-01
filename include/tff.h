#ifndef TFF_H
#define TFF_H

#include "BTS.h"

namespace tff {

constexpr int stackLength = 2 * BTS::maxValue + 1;

BTS::Tryte _stack[stackLength];
BTS::Tryte *stack = &_stack[BTS::maxValue];

}; // namespace tff

#endif