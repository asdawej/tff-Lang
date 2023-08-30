#include "Stream.h"

namespace Stream {

// === StackStream ===

Type_Stream StackStream::type() { return Type_Stream::Type_StackStream; }

void StackStream::in(BTS::Tryte data) {
    *stackAddr = data;
    stackAddr++;
}

BTS::Tryte StackStream::out() {
    auto &ret = *stackAddr;
    stackAddr++;
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

// === 标准流包装 ===

BTS::Tryte _std_func_in(std::istream &istr) {
    std::cout << "[in] ";
    int _val;
    istr >> _val;
    return BTS::Tryte(_val);
}

void _std_func_out(BTS::Tryte tr, std::ostream &ostr) {
    std::cout << "[out] ";
    ostr << (int)tr << std::endl;
}

IstreamStd stdinStream(std::cin, _std_func_in);
OstreamStd stdoutStream(std::cout, _std_func_out);
OstreamStd stderrStream(std::cerr, _std_func_out);

}; // namespace Stream