#include "../include/BTS.h"

namespace BTS {

char trit2char(Trit t) {
    if (t.fbit) {
        if (t.lbit)
            return 'T';
        else
            return 'N';
    } else {
        if (t.lbit)
            return 'P';
        else
            return 'F';
    }
}

Trit char2trit(char c) {
    Trit t;
    switch (c) {
    case 'T':
    case 't':
        t.fbit = true;
        t.lbit = true;
        break;
    case 'N':
    case 'n':
    default:
        t.fbit = true;
        t.lbit = false;
        break;
    case 'F':
    case 'f':
        t.fbit = false;
        t.lbit = false;
        break;
    case 'P':
    case 'p':
        t.fbit = false;
        t.lbit = true;
        break;
    }
    return t;
}

Tryte::Tryte(const char s[tryteTritNum]) {
    for (register int i = 0; i < tryteTritNum; i++) {
        Trit t = char2trit(s[i]);
        value[2 * i] = t.fbit;
        value[2 * i + 1] = t.lbit;
    }
}

char *Tryte::repr() {
    char *ret = new char[9];
    for (register int i = 0; i < tryteTritNum; i++)
        ret[i] = trit2char(Trit(value[2 * i], value[2 * i + 1]));
    ret[8] = '\0';
    return ret;
}

Tryte::operator int() {
    int ret = 0;
    for (register int i = 0; i < tryteTritNum; i++) {
        ret *= 2;
        switch (trit2char(Trit(value[2 * i], value[2 * i + 1]))) {
        case 'T':
            ret += 1;
            break;
        case 'F':
            ret -= 1;
            break;
        case 'N':
        case 'P':
            break;
        }
    }
    return ret;
}

}; // namespace BTS