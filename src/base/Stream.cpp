#include "Stream.h"
#include "tff.h"

namespace Stream {

// 辅助函数, 取表达式值
BTS::Tryte _getValue(tff::TryteExpr expr) {
    auto count = expr.suffix;
    auto ret = expr.value;
    while (count > 0) {
        ret = tff::stack[ret];
        count--;
    }
    return ret;
}

// === StackStream ===

Type_Stream StackStream::type() { return Type_Stream::Type_StackStream; }

void StackStream::in(BTS::Tryte data) {
    tff::stack[_getValue(stackAddr) + size] = data;
    size++;
}

BTS::Tryte StackStream::out() {
    auto &ret = tff::stack[_getValue(stackAddr) + size];
    size++;
    return ret;
}

// === IstreamStd ===

Type_Stream IstreamStd::type() { return Type_Stream::Type_IstreamStd; }

BTS::Tryte IstreamStd::out() {
    if (func_in != nullptr)
        return func_in(*istr);
    char s[BTS::tryteTritNum];
    *istr >> s;
    return BTS::Tryte(s);
}

// === OstreamStd ===

Type_Stream OstreamStd::type() { return Type_Stream::Type_OstreamStd; }

void OstreamStd::in(BTS::Tryte tr) {
    if (func_out != nullptr)
        return func_out(tr, *ostr);
    *ostr << tr.repr();
}

}; // namespace Stream