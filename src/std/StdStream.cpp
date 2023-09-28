#include "std/StdStream.h"

namespace tff {

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

Stream::IstreamStd stdinStream(std::cin, _std_func_in);
Stream::OstreamStd stdoutStream(std::cout, _std_func_out);
Stream::OstreamStd stderrStream(std::cerr, _std_func_out);

}; // namespace tff