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
    char s[BTS::tryteTritNum];
    *istr >> s;
    return BTS::Tryte(s);
}

void OstreamStd::in(BTS::Tryte tr) { *ostr << tr.repr(); }

IstreamStd stdinStream(std::cin);
OstreamStd stdoutStream(std::cout);
OstreamStd stderrStream(std::cerr);

}; // namespace Stream