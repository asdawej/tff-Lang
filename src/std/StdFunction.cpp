#include "std/StdFunction.h"
#include "tff.h"
#include <chrono>

#ifdef __MINGW32__ // https://github.com/meganz/mingw-std-threads
#include <mingw.thread.h>
#else
#include <thread>
#endif

namespace tff {

void add() { tff::stack[0] = tff::stack[1] + tff::stack[2]; }
void sub() { tff::stack[0] = tff::stack[1] - tff::stack[2]; }
void mul() { tff::stack[0] = tff::stack[1] * tff::stack[2]; }
void div() { tff::stack[0] = tff::stack[1] / tff::stack[2]; }
void mod() { tff::stack[0] = tff::stack[1] % tff::stack[2]; }

void sleep() { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }

}; // namespace tff