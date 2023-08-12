#ifndef STREAM_H
#define STREAM_H

#include "BTS.h"
#include <iostream>

namespace Stream {

// 输入流接口
struct Istream {
    Istream() = default;
    ~Istream() = default;
    virtual void in(BTS::Tryte) = 0;
};

// 输出流接口
struct Ostream {
    Ostream() = default;
    ~Ostream() = default;
    virtual BTS::Tryte out() = 0;
};

// 内存栈数据流
struct StackStream : public Istream, public Ostream {
    StackStream() = default;
    StackStream(BTS::Tryte &_stackAddr) : stackAddr(&_stackAddr){};
    ~StackStream() = default;
    BTS::Tryte *stackAddr; // 内存栈地址
    void in(BTS::Tryte);
    BTS::Tryte out();
};

// istream包装
struct IstreamStd : public Ostream {
    IstreamStd() = default;
    IstreamStd(std::istream &_istr) : istr(&_istr){};
    ~IstreamStd() = default;
    std::istream *istr; // 输入流
    BTS::Tryte out();
};

// ostream包装
struct OstreamStd : public Istream {
    OstreamStd() = default;
    OstreamStd(std::ostream &_ostr) : ostr(&_ostr){};
    ~OstreamStd() = default;
    std::ostream *ostr; // 输出流
    void in(BTS::Tryte);
};

extern IstreamStd stdinStream;  // stdin包装
extern OstreamStd stdoutStream; // stdout包装
extern OstreamStd stderrStream; // stderr包装

}; // namespace Stream

#endif