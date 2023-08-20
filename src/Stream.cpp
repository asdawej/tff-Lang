#include "../include/Stream.h"

namespace Stream {

void StackStream::in(BTS::Tryte data) {
    *stackAddr = data;
    stackAddr++;
}

BTS::Tryte StackStream::out() {
    auto &ret = *stackAddr;
    stackAddr++;
    return ret;
}

BTS::Tryte IstreamStd::out() {
    if (func_in != nullptr)
        return func_in(*istr);
    char s[BTS::tryteTritNum];
    *istr >> s;
    return BTS::Tryte(s);
}

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