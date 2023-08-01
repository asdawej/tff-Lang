#ifndef _ASSISTFUNC_H
#define _ASSISTFUNC_H

namespace AssistFunc {

constexpr int pow2(int x) {
    int ret = 1;
    for (int i = 0; i < x; i++)
        ret *= 2;
    return ret;
}

}; // namespace AssistFunc

#endif