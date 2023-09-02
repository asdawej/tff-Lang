#include "BTS.h"

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
    for (auto i = 0; i < tryteTritNum; i++) {
        Trit t = char2trit(s[i]);
        value[2 * i] = t.fbit;
        value[2 * i + 1] = t.lbit;
    }
}

Tryte::Tryte(const int &i) {
    if (i == 0) {
        Trit t = char2trit('N');
        for (auto m = 0; m < tryteTritNum; m++) {
            value[2 * m] = t.fbit;
            value[2 * m + 1] = t.lbit;
        }
    } else if (i > 0) {
        if (i > maxValue) {
            Trit t = char2trit('T');
            for (auto m = 0; m < tryteTritNum; m++) {
                value[2 * m] = t.fbit;
                value[2 * m + 1] = t.lbit;
            }
            return;
        }
        int _i = i;
        for (auto m = tryteTritNum - 1; m >= 0; m--) {
            Trit t = ((_i % 2) ? char2trit('T') : char2trit('N'));
            value[2 * m] = t.fbit;
            value[2 * m + 1] = t.lbit;
            _i /= 2;
        }
    } else if (i < 0) {
        if (i < -maxValue) {
            Trit t = char2trit('F');
            for (auto m = 0; m < tryteTritNum; m++) {
                value[2 * m] = t.fbit;
                value[2 * m + 1] = t.lbit;
            }
            return;
        }
        int _i = -i;
        for (auto m = tryteTritNum - 1; m >= 0; m--) {
            Trit t = ((_i % 2) ? char2trit('F') : char2trit('N'));
            value[2 * m] = t.fbit;
            value[2 * m + 1] = t.lbit;
            _i /= 2;
        }
    }
}

char *Tryte::repr() {
    char *ret = new char[tryteTritNum + 1];
    for (auto i = 0; i < tryteTritNum; i++)
        ret[i] = trit2char(Trit(value[2 * i], value[2 * i + 1]));
    ret[tryteTritNum] = '\0';
    return ret;
}

Tryte::operator int() {
    int ret = 0;
    for (auto i = 0; i < tryteTritNum; i++) {
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