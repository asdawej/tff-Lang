#ifndef BTS_H
#define BTS_H

#include "AssistFunc.h"
#include <bitset>

namespace BTS {

constexpr int tryteTritNum = 8;
constexpr int maxValue = AssistFunc::pow2(tryteTritNum) - 1;

struct Trit {
    Trit() : fbit(false), lbit(false){};
    Trit(bool _f, bool _l) : fbit(_f), lbit(_l){};
    ~Trit() = default;
    bool fbit;
    bool lbit;
};

char trit2char(Trit t);

Trit char2trit(char c);

struct Tryte {
    Tryte() = default;
    Tryte(const char s[tryteTritNum]);
    Tryte(const int &);
    ~Tryte() = default;
    std::bitset<2 * tryteTritNum> value;
    char *repr();
    operator int();
};

}; // namespace BTS

#endif