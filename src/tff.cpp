#include "tff.h"

namespace tff {

BTS::Tryte _stack[stackLength];
BTS::Tryte *stack = &_stack[BTS::maxValue];

funcTree _funcTable[stackLength];
funcTree *funcTable = &_funcTable[BTS::maxValue];

TryteExpr::TryteExpr(const char *s) {
    char s_value[BTS::tryteTritNum];
    char *ptr = (char *)s;
    auto count_value = 0;
    while (*ptr) {
        if (*ptr == '2')
            break;
        count_value++;
        ptr++;
    }
    auto count_suffix = 0;
    while (*ptr) {
        if (*ptr == '2')
            count_suffix++;
        ptr++;
    }
    for (auto i = 0; i < BTS::tryteTritNum; i++) {
        if (i < BTS::tryteTritNum - count_value)
            s_value[i] = 'N';
        else
            s_value[i] = s[i - (BTS::tryteTritNum - count_value)];
    }
    value = BTS::Tryte(s_value);
    suffix = count_suffix;
}

char *TryteExpr::repr() {
    char *ret = new char[BTS::tryteTritNum + 1 + suffix];
    char *s_value = value.repr();
    std::copy(s_value, s_value + BTS::tryteTritNum, ret);
    delete[] s_value;
    for (auto i = BTS::tryteTritNum; i < BTS::tryteTritNum + suffix; i++)
        ret[i] = '2';
    ret[BTS::tryteTritNum + suffix] = '\0';
    return ret;
}

TryteExpr::operator int() {
    auto ret = value;
    auto count = suffix;
    while (count > 0) {
        ret = tff::stack[ret];
        count--;
    }
    return (int)ret;
}

TryteExpr::operator BTS::Tryte() {
    auto ret = value;
    auto count = suffix;
    while (count > 0) {
        ret = tff::stack[ret];
        count--;
    }
    return ret;
}

}; // namespace tff